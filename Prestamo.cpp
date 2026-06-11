#include "Prestamo.h"

Prestamo::Prestamo(const QString &cliente,
                   const QString &ci,
                   double monto,
                   int dias,
                   double porcentajeInteres,
                   QObject *parent)
    : OperacionFinanciera(parent)
    , m_cliente(cliente)
    , m_ci(ci)
    , m_monto(monto)
    , m_dias(dias)
    , m_porcentajeInteres(porcentajeInteres)
{
}

Prestamo::~Prestamo() = default;

double Prestamo::calcularMontoTotal() const
{
    // Calcula el monto total con interés sobre los días informados.
    return m_monto + (m_monto * m_porcentajeInteres * m_dias / 100.0);
}

void Prestamo::actualizarEstado(const QString &nuevoEstado)
{
    m_estado = nuevoEstado;
}

QString Prestamo::cliente() const
{
    return m_cliente;
}

void Prestamo::setCliente(const QString &cliente)
{
    m_cliente = cliente;
}

QString Prestamo::ci() const
{
    return m_ci;
}

void Prestamo::setCi(const QString &ci)
{
    m_ci = ci;
}

double Prestamo::monto() const
{
    return m_monto;
}

void Prestamo::setMonto(double monto)
{
    m_monto = monto;
}

int Prestamo::dias() const
{
    return m_dias;
}

void Prestamo::setDias(int dias)
{
    m_dias = dias;
}

double Prestamo::porcentajeInteres() const
{
    return m_porcentajeInteres;
}

void Prestamo::setPorcentajeInteres(double porcentajeInteres)
{
    m_porcentajeInteres = porcentajeInteres;
}

bool Prestamo::operator<(const Prestamo &other) const
{
    return calcularMontoTotal() < other.calcularMontoTotal();
}
