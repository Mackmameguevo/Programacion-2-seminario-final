#include "mainwindow.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
int main(int argc, char *argv[])
{
    qDebug() << QSqlDatabase::drivers();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
