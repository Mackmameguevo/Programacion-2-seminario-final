#include "GestionBanco.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

GestionBanco::GestionBanco(QObject *parent)
    : QObject(parent), m_efectivoDisponible(50000.0), m_maxPrestamosCliente(3), m_montoMaximoPrestamo(10000.0)
{
}

GestionBanco::~GestionBanco()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    qDeleteAll(m_prestamos);
    m_prestamos.clear();
}

bool GestionBanco::conectarBaseDatos(const QString &host, const QString &usuario,
                                     const QString &contrasena, const QString &baseDatos, int puerto)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName(host);
    m_db.setUserName(usuario);
    m_db.setPassword(contrasena);
    m_db.setDatabaseName(baseDatos);
    m_db.setPort(puerto);

    if (!m_db.open()) {
        establecerError("Error al conectar con la base de datos: " + m_db.lastError().text());
        return false;
    }

    return true;
}

bool GestionBanco::inicializarBaseDatos()
{
    if (!m_db.isOpen()) {
        establecerError("La base de datos no está conectada");
        return false;
    }

    QSqlQuery query;

    // Crear tabla de préstamos
    if (!query.exec("CREATE TABLE IF NOT EXISTS Prestamos ("
                    "id INT AUTO_INCREMENT PRIMARY KEY,"
                    "cliente VARCHAR(100) NOT NULL,"
                    "ci VARCHAR(30) NOT NULL UNIQUE,"
                    "monto DOUBLE NOT NULL,"
                    "dias INT NOT NULL,"
                    "porcentaje_interes DOUBLE NOT NULL,"
                    "monto_total DOUBLE NOT NULL,"
                    "estado VARCHAR(50) DEFAULT 'Activo',"
                    "fecha_creacion TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                    ")")) {
        establecerError("Error al crear tabla Prestamos: " + query.lastError().text());
        return false;
    }

    return true;
}

bool GestionBanco::cargarPrestamosDesdeDB()
{
    if (!m_db.isOpen()) {
        establecerError("La base de datos no está conectada");
        return false;
    }

    QSqlQuery query("SELECT cliente, ci, monto, dias, porcentaje_interes, estado FROM Prestamos WHERE estado='Activo'");

    if (!query.exec()) {
        establecerError("Error al cargar préstamos: " + query.lastError().text());
        return false;
    }

    qDeleteAll(m_prestamos);
    m_prestamos.clear();

    while (query.next()) {
        Prestamo *prestamo = new Prestamo(
            query.value(0).toString(),      // cliente
            query.value(1).toString(),      // ci
            query.value(2).toDouble(),      // monto
            query.value(3).toInt(),         // dias
            query.value(4).toDouble(),      // porcentaje_interes
            this
        );
        prestamo->actualizarEstado(query.value(5).toString());
        m_prestamos.append(prestamo);
    }

    return true;
}

void GestionBanco::registrarPrestamo(Prestamo *prestamo)
{
    if (prestamo) {
        m_prestamos.append(prestamo);
    }
}

bool GestionBanco::guardarPrestamoDB(Prestamo *prestamo)
{
    if (!m_db.isOpen() || !prestamo) {
        establecerError("Base de datos no conectada o préstamo inválido");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Prestamos (cliente, ci, monto, dias, porcentaje_interes, monto_total, estado) "
                  "VALUES (:cliente, :ci, :monto, :dias, :interes, :monto_total, 'Activo')");

    query.addBindValue(prestamo->cliente());
    query.addBindValue(prestamo->ci());
    query.addBindValue(prestamo->monto());
    query.addBindValue(prestamo->dias());
    query.addBindValue(prestamo->porcentajeInteres());
    query.addBindValue(prestamo->calcularMontoTotal());

    if (!query.exec()) {
        establecerError("Error al guardar préstamo: " + query.lastError().text());
        return false;
    }

    return true;
}

bool GestionBanco::eliminarPrestamo(const QString &ci)
{
    for (int i = 0; i < m_prestamos.size(); ++i) {
        if (m_prestamos.at(i)->ci() == ci) {
            delete m_prestamos.takeAt(i);
            return true;
        }
    }
    return false;
}

bool GestionBanco::eliminarPrestamoDelDB(const QString &ci)
{
    if (!m_db.isOpen()) {
        establecerError("Base de datos no conectada");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE Prestamos SET estado='Cancelado' WHERE ci = :ci");
    query.addBindValue(ci);

    if (!query.exec()) {
        establecerError("Error al eliminar préstamo: " + query.lastError().text());
        return false;
    }

    return eliminarPrestamo(ci);
}

double GestionBanco::calcularMontoTotalGeneral() const
{
    double total = 0.0;
    for (const Prestamo *prestamo : qAsConst(m_prestamos)) {
        total += prestamo->calcularMontoTotal();
    }
    return total;
}

double GestionBanco::calcularMontoTotalPorCi(const QString &ci) const
{
    const Prestamo *prestamo = buscarPrestamoPorCi(ci);
    return prestamo ? prestamo->calcularMontoTotal() : 0.0;
}

double GestionBanco::calcularDineroCirculante() const
{
    double total = 0.0;
    for (const Prestamo *prestamo : qAsConst(m_prestamos)) {
        total += prestamo->monto();
    }
    return total;
}

double GestionBanco::calcularMontoTotalARecuperar(int diasPeriodo) const
{
    if (diasPeriodo <= 0) {
        return 0.0;
    }

    double total = 0.0;
    for (const Prestamo *prestamo : qAsConst(m_prestamos)) {
        if (prestamo->dias() <= diasPeriodo) {
            total += prestamo->calcularMontoTotal();
        }
    }
    return total;
}

int GestionBanco::contarPrestamosCliente(const QString &ci) const
{
    int contador = 0;
    for (const Prestamo *prestamo : qAsConst(m_prestamos)) {
        if (prestamo->ci() == ci) {
            contador++;
        }
    }
    return contador;
}

void GestionBanco::aplicarReglasDeInteres()
{
    for (int i = 0; i < m_prestamos.size(); ++i) {
        Prestamo *prestamo = m_prestamos.at(i);
        int numPrestamosCliente = contarPrestamosCliente(prestamo->ci());

        if (numPrestamosCliente == 2) {
            prestamo->setPorcentajeInteres(prestamo->porcentajeInteres() * 2.0);
        } else if (numPrestamosCliente >= 3) {
            prestamo->setPorcentajeInteres(prestamo->porcentajeInteres() * 3.0);
        }
    }
}

bool GestionBanco::validarNuevoPrestamo(const QString &ci, double monto) const
{
    if (monto > m_montoMaximoPrestamo) {
        establecerError("El monto solicitado excede el límite máximo de $" + QString::number(m_montoMaximoPrestamo));
        return false;
    }

    if (contarPrestamosCliente(ci) >= m_maxPrestamosCliente) {
        establecerError("El cliente ya tiene el máximo número de préstamos permitidos (3)");
        return false;
    }

    if (monto > m_efectivoDisponible) {
        establecerError("Fondos insuficientes en la institución bancaria");
        return false;
    }

    return true;
}

QList<Prestamo *> GestionBanco::prestamos() const
{
    return m_prestamos;
}

QString GestionBanco::ultimoError() const
{
    return m_ultimoError;
}

Prestamo *GestionBanco::buscarPrestamoPorCi(const QString &ci) const
{
    for (Prestamo *prestamo : m_prestamos) {
        if (prestamo->ci() == ci) {
            return prestamo;
        }
    }
    return nullptr;
}

void GestionBanco::establecerError(const QString &error) const
{
    m_ultimoError = error;
    qDebug() << "Error GestionBanco:" << error;
}
