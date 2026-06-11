#include "GestionBanco.h"

GestionBanco::GestionBanco(QObject *parent)
    : QObject(parent)
{
}

GestionBanco::~GestionBanco()
{
    qDeleteAll(m_prestamos);
    m_prestamos.clear();
}

void GestionBanco::registrarPrestamo(Prestamo *prestamo)
{
    if (prestamo) {
        m_prestamos.append(prestamo);
    }
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

void GestionBanco::aplicarReglasDeInteres()
{
    for (int i = 0; i < m_prestamos.size(); ++i) {
        Prestamo *prestamo = m_prestamos.at(i);
        if (i == 1) {
            prestamo->setPorcentajeInteres(prestamo->porcentajeInteres() * 2.0);
        } else if (i == 2) {
            prestamo->setPorcentajeInteres(prestamo->porcentajeInteres() * 3.0);
        }
    }
}

QList<Prestamo *> GestionBanco::prestamos() const
{
    return m_prestamos;
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
