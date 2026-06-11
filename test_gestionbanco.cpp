#include <QtTest/QtTest>
#include "GestionBanco.h"
#include "Prestamo.h"

class TestGestionBanco : public QObject
{
    Q_OBJECT

private slots:
    void testRegistrarPrestamo();
    void testEliminarPrestamo();
    void testCalcularMontoTotalPrestamo();
    void testCalcularMontoTotalARecuperarPorPeriodo();
    void testAplicarReglasDeInteres();
    void testPeriodoLimiteCero();
};

void TestGestionBanco::testRegistrarPrestamo()
{
    GestionBanco banco;
    Prestamo *prestamo = new Prestamo("Ana Gomez", "98765432", 1000.0, 30, 1.2, &banco);

    banco.registrarPrestamo(prestamo);

    QCOMPARE(banco.prestamos().size(), 1);
    QCOMPARE(banco.prestamos().first()->ci(), QString("98765432"));
}

void TestGestionBanco::testEliminarPrestamo()
{
    GestionBanco banco;
    banco.registrarPrestamo(new Prestamo("Carlos Diaz", "11122233", 500.0, 15, 1.0, &banco));
    banco.registrarPrestamo(new Prestamo("Lucia Ruiz", "44455566", 800.0, 20, 1.5, &banco));

    bool eliminado = banco.eliminarPrestamo("11122233");
    QVERIFY(eliminado);
    QCOMPARE(banco.prestamos().size(), 1);
    QCOMPARE(banco.prestamos().first()->ci(), QString("44455566"));
}

void TestGestionBanco::testCalcularMontoTotalPrestamo()
{
    Prestamo prestamo("Mario Lopez", "55566677", 1000.0, 30, 1.0);
    double montoTotal = prestamo.calcularMontoTotal();

    QCOMPARE(montoTotal, 1300.0);
}

void TestGestionBanco::testCalcularMontoTotalARecuperarPorPeriodo()
{
    GestionBanco banco;
    banco.registrarPrestamo(new Prestamo("Pedro Ramos", "22233344", 1000.0, 15, 1.0, &banco));
    banco.registrarPrestamo(new Prestamo("Sofia Mora", "33344455", 2000.0, 45, 1.0, &banco));

    double recuperar30Dias = banco.calcularMontoTotalARecuperar(30);
    QCOMPARE(recuperar30Dias, 1150.0);

    double recuperar60Dias = banco.calcularMontoTotalARecuperar(60);
    QCOMPARE(recuperar60Dias, 3150.0);
}

void TestGestionBanco::testAplicarReglasDeInteres()
{
    GestionBanco banco;
    banco.registrarPrestamo(new Prestamo("Ana Vega", "66677788", 1000.0, 10, 1.0, &banco));
    banco.registrarPrestamo(new Prestamo("Diego Perez", "77788899", 1000.0, 10, 1.0, &banco));
    banco.registrarPrestamo(new Prestamo("Elena Soto", "88899900", 1000.0, 10, 1.0, &banco));

    banco.aplicarReglasDeInteres();

    QCOMPARE(banco.prestamos().at(0)->porcentajeInteres(), 1.0);
    QCOMPARE(banco.prestamos().at(1)->porcentajeInteres(), 2.0);
    QCOMPARE(banco.prestamos().at(2)->porcentajeInteres(), 3.0);
}

void TestGestionBanco::testPeriodoLimiteCero()
{
    GestionBanco banco;
    banco.registrarPrestamo(new Prestamo("Mariana Cruz", "99900011", 1000.0, 30, 1.0, &banco));

    QCOMPARE(banco.calcularMontoTotalARecuperar(0), 0.0);
}

QTEST_APPLESS_MAIN(TestGestionBanco)

#include "test_gestionbanco.moc"
