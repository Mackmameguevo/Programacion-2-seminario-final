#include "OperacionFinanciera.h"

OperacionFinanciera::OperacionFinanciera(QObject *parent)
    : QObject(parent)
    , m_estado()
{
}

OperacionFinanciera::~OperacionFinanciera() = default;

QString OperacionFinanciera::estado() const
{
    return m_estado;
}
