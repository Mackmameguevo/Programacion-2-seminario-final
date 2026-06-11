#ifndef GESTIONBANCO_H
#define GESTIONBANCO_H

#include <QObject>
#include <QList>
#include <QString>
#include "Prestamo.h"

class GestionBanco : public QObject
{
    Q_OBJECT

public:
    explicit GestionBanco(QObject *parent = nullptr);
    ~GestionBanco() override;

    void registrarPrestamo(Prestamo *prestamo);
    bool eliminarPrestamo(const QString &ci);

    double calcularMontoTotalGeneral() const;
    double calcularMontoTotalPorCi(const QString &ci) const;
    double calcularDineroCirculante() const;
    double calcularMontoTotalARecuperar(int diasPeriodo) const;

    void aplicarReglasDeInteres();

    QList<Prestamo *> prestamos() const;

private:
    Prestamo *buscarPrestamoPorCi(const QString &ci) const;

    QList<Prestamo *> m_prestamos;
};

#endif // GESTIONBANCO_H
