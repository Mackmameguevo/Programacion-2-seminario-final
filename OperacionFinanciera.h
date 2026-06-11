#ifndef OPERACIONFINANCIERA_H
#define OPERACIONFINANCIERA_H

#include <QObject>
#include <QString>

class OperacionFinanciera : public QObject
{
    Q_OBJECT

public:
    explicit OperacionFinanciera(QObject *parent = nullptr);
    ~OperacionFinanciera() override;

    virtual double calcularMontoTotal() const = 0;
    virtual void actualizarEstado(const QString &nuevoEstado) = 0;

    QString estado() const;

protected:
    QString m_estado;
};

#endif // OPERACIONFINANCIERA_H
