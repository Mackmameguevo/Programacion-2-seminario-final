#include <QCoreApplication>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("banco");
    db.setUserName("root");
    db.setPassword("password");
    db.setPort(3306);

    if (!db.open()) {
        qDebug() << "Error al conectar con la base de datos:" << db.lastError().text();
        return 1;
    }

    QSqlQuery query;

    // Crear la tabla Prestamos si no existe
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Prestamos ("
            "id INT AUTO_INCREMENT PRIMARY KEY,"
            "cliente VARCHAR(100) NOT NULL,"
            "ci VARCHAR(30) NOT NULL UNIQUE,"
            "monto DOUBLE NOT NULL,"
            "dias INT NOT NULL,"
            "porcentajeInteres DOUBLE NOT NULL,"
            "estado VARCHAR(50) DEFAULT 'pendiente'"
            ") ENGINE=InnoDB;")) {
        qDebug() << "Error al crear la tabla Prestamos:" << query.lastError().text();
        return 1;
    }

    // INSERT
    if (!query.prepare(
            "INSERT INTO Prestamos (cliente, ci, monto, dias, porcentajeInteres, estado) "
            "VALUES (:cliente, :ci, :monto, :dias, :porcentajeInteres, :estado)")) {
        qDebug() << "Error al preparar INSERT:" << query.lastError().text();
        return 1;
    }

    query.bindValue(":cliente", "Juan Perez");
    query.bindValue(":ci", "12345678");
    query.bindValue(":monto", 15000.0);
    query.bindValue(":dias", 30);
    query.bindValue(":porcentajeInteres", 1.5);
    query.bindValue(":estado", "activo");

    if (!query.exec()) {
        qDebug() << "Error al ejecutar INSERT:" << query.lastError().text();
    }

    // UPDATE
    if (!query.prepare(
            "UPDATE Prestamos SET porcentajeInteres = :porcentajeInteres, estado = :estado "
            "WHERE ci = :ci")) {
        qDebug() << "Error al preparar UPDATE:" << query.lastError().text();
        return 1;
    }

    query.bindValue(":porcentajeInteres", 2.0);
    query.bindValue(":estado", "actualizado");
    query.bindValue(":ci", "12345678");

    if (!query.exec()) {
        qDebug() << "Error al ejecutar UPDATE:" << query.lastError().text();
    }

    // DELETE
    if (!query.prepare("DELETE FROM Prestamos WHERE ci = :ci")) {
        qDebug() << "Error al preparar DELETE:" << query.lastError().text();
        return 1;
    }
    query.bindValue(":ci", "12345678");

    if (!query.exec()) {
        qDebug() << "Error al ejecutar DELETE:" << query.lastError().text();
    }

    // SELECT
    if (!query.exec("SELECT id, cliente, ci, monto, dias, porcentajeInteres, estado FROM Prestamos")) {
        qDebug() << "Error al ejecutar SELECT:" << query.lastError().text();
        return 1;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString cliente = query.value(1).toString();
        QString ci = query.value(2).toString();
        double monto = query.value(3).toDouble();
        int dias = query.value(4).toInt();
        double porcentajeInteres = query.value(5).toDouble();
        QString estado = query.value(6).toString();

        qDebug() << "ID:" << id
                 << "Cliente:" << cliente
                 << "CI:" << ci
                 << "Monto:" << monto
                 << "Dias:" << dias
                 << "Interés:" << porcentajeInteres
                 << "Estado:" << estado;
    }

    db.close();
    return 0;
}
