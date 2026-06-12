#ifndef GESTIONBANCO_H
#define GESTIONBANCO_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSqlDatabase>
#include "Prestamo.h"

class GestionBanco : public QObject
{
    Q_OBJECT

public:
    explicit GestionBanco(QObject *parent = nullptr);
    ~GestionBanco() override;

    bool conectarBaseDatos(const QString &host, const QString &usuario, 
                          const QString &contrasena, const QString &baseDatos, int puerto = 3306);
    bool inicializarBaseDatos();
    bool cargarPrestamosDesdeDB();

    void registrarPrestamo(Prestamo *prestamo);
    bool guardarPrestamoDB(Prestamo *prestamo);
    bool eliminarPrestamo(const QString &ci);
    bool eliminarPrestamoDelDB(const QString &ci);

    double calcularMontoTotalGeneral() const;
    double calcularMontoTotalPorCi(const QString &ci) const;
    double calcularDineroCirculante() const;
    double calcularMontoTotalARecuperar(int diasPeriodo) const;
    int contarPrestamosCliente(const QString &ci) const;

    void aplicarReglasDeInteres();
    bool validarNuevoPrestamo(const QString &ci, double monto) const;

    QList<Prestamo *> prestamos() const;
    QString ultimoError() const;

private:
    Prestamo *buscarPrestamoPorCi(const QString &ci) const;
    void establecerError(const QString &error) const;

    QList<Prestamo *> m_prestamos;
    QSqlDatabase m_db;
    mutable QString m_ultimoError;
    double m_efectivoDisponible;
    int m_maxPrestamosCliente;
    double m_montoMaximoPrestamo;
};

#endif // GESTIONBANCO_H
