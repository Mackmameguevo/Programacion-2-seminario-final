#ifndef VALIDACIONPRESTAMO_H
#define VALIDACIONPRESTAMO_H

#include <QObject>
#include <QString>
#include <QWidget>

class ValidacionPrestamo : public QObject
{
    Q_OBJECT

public:
    explicit ValidacionPrestamo(QObject *parent = nullptr);

    static bool validarNombre(const QString &nombre, QWidget *parent = nullptr);
    static bool validarCi(const QString &ci, QWidget *parent = nullptr);
    static bool validarMonto(const QString &monto, QWidget *parent = nullptr);
    static bool validarDias(const QString &dias, QWidget *parent = nullptr);
    static bool validarPorcentajeInteres(const QString &porcentajeInteres, QWidget *parent = nullptr);
    static bool validarPrestamo(const QString &cliente,
                               const QString &ci,
                               const QString &monto,
                               const QString &dias,
                               const QString &porcentajeInteres,
                               QWidget *parent = nullptr);
};

#endif // VALIDACIONPRESTAMO_H
