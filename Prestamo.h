#ifndef PRESTAMO_H
#define PRESTAMO_H

#include "OperacionFinanciera.h"
#include <QString>

class Prestamo : public OperacionFinanciera
{
    Q_OBJECT

public:
    explicit Prestamo(const QString &cliente,
                      const QString &ci,
                      double monto,
                      int dias,
                      double porcentajeInteres,
                      QObject *parent = nullptr);
    ~Prestamo() override;

    double calcularMontoTotal() const override;
    void actualizarEstado(const QString &nuevoEstado) override;

    QString cliente() const;
    void setCliente(const QString &cliente);

    QString ci() const;
    void setCi(const QString &ci);

    double monto() const;
    void setMonto(double monto);

    int dias() const;
    void setDias(int dias);

    double porcentajeInteres() const;
    void setPorcentajeInteres(double porcentajeInteres);

    bool operator<(const Prestamo &other) const;

private:
    QString m_cliente;
    QString m_ci;
    double m_monto;
    int m_dias;
    double m_porcentajeInteres;
};

#endif // PRESTAMO_H
