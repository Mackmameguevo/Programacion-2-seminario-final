#include "ValidacionPrestamo.h"
#include <QDoubleValidator>
#include <QIntValidator>
#include <QMessageBox>
#include <QRegularExpressionValidator>

ValidacionPrestamo::ValidacionPrestamo(QObject *parent)
    : QObject(parent)
{
}

static void mostrarError(const QString &mensaje, QWidget *parent)
{
    QMessageBox::warning(parent, QObject::tr("Error de validación"), mensaje);
}

bool ValidacionPrestamo::validarNombre(const QString &nombre, QWidget *parent)
{
    QRegularExpressionValidator validator(QRegularExpression("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]{3,100}$"));
    int pos = 0;
    if (validator.validate(const_cast<QString &>(nombre), pos) != QValidator::Acceptable) {
        mostrarError(QObject::tr("Nombre inválido. Debe tener entre 3 y 100 caracteres y solo contener letras y espacios."), parent);
        return false;
    }
    return true;
}

bool ValidacionPrestamo::validarCi(const QString &ci, QWidget *parent)
{
    QRegularExpressionValidator validator(QRegularExpression("^\\d{6,12}$"));
    int pos = 0;
    if (validator.validate(const_cast<QString &>(ci), pos) != QValidator::Acceptable) {
        mostrarError(QObject::tr("CI inválida. Debe contener entre 6 y 12 dígitos."), parent);
        return false;
    }
    return true;
}

bool ValidacionPrestamo::validarMonto(const QString &monto, QWidget *parent)
{
    QDoubleValidator validator(0.01, 999999999.99, 2);
    validator.setNotation(QDoubleValidator::StandardNotation);
    int pos = 0;
    if (validator.validate(const_cast<QString &>(monto), pos) != QValidator::Acceptable) {
        mostrarError(QObject::tr("Monto inválido. Ingrese un valor numérico positivo con hasta 2 decimales."), parent);
        return false;
    }
    return true;
}

bool ValidacionPrestamo::validarDias(const QString &dias, QWidget *parent)
{
    QIntValidator validator(1, 3650);
    int pos = 0;
    if (validator.validate(const_cast<QString &>(dias), pos) != QValidator::Acceptable) {
        mostrarError(QObject::tr("Días inválidos. Ingrese un número entero positivo entre 1 y 3650."), parent);
        return false;
    }
    return true;
}

bool ValidacionPrestamo::validarPorcentajeInteres(const QString &porcentajeInteres, QWidget *parent)
{
    QDoubleValidator validator(0.0, 100.0, 2);
    validator.setNotation(QDoubleValidator::StandardNotation);
    int pos = 0;
    if (validator.validate(const_cast<QString &>(porcentajeInteres), pos) != QValidator::Acceptable) {
        mostrarError(QObject::tr("Porcentaje de interés inválido. Ingrese un valor numérico entre 0 y 100."), parent);
        return false;
    }
    return true;
}

bool ValidacionPrestamo::validarPrestamo(const QString &cliente,
                                         const QString &ci,
                                         const QString &monto,
                                         const QString &dias,
                                         const QString &porcentajeInteres,
                                         QWidget *parent)
{
    if (!validarNombre(cliente, parent)) {
        return false;
    }
    if (!validarCi(ci, parent)) {
        return false;
    }
    if (!validarMonto(monto, parent)) {
        return false;
    }
    if (!validarDias(dias, parent)) {
        return false;
    }
    if (!validarPorcentajeInteres(porcentajeInteres, parent)) {
        return false;
    }
    return true;
}
