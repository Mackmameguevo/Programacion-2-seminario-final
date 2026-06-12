#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <memory>
#include "../GestionBanco.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNuevoPrestamo();
    void onEliminarPrestamo();
    void onActualizarTabla();
    void onCargarEstadisticas();
    void onConsultarPorPeriodo();

private:
    void inicializarUI();
    void configurarBaseDatos();
    void actualizarTablaPrestamos();
    void mostrarMensaje(const QString &titulo, const QString &mensaje, bool esError = false);
    void cargarPrestamos();

    Ui::MainWindow *ui;
    std::unique_ptr<GestionBanco> m_gestionBanco;
    
    // Controles principales
    QTableWidget *m_tablaPrestamos;
    QLabel *m_lblMontoTotal;
    QLabel *m_lblDineroCirculante;
    QLabel *m_lblNumPrestamos;
    QLabel *m_lblResultadoPeriodo;
    QSpinBox *m_spinDias;
};

#endif // MAINWINDOW_H
