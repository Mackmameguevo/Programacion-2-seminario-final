#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>
#include <QTabWidget>
#include <QHeaderView>
#include <QValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_gestionBanco(std::make_unique<GestionBanco>())
    , m_tablaPrestamos(nullptr)
    , m_lblMontoTotal(nullptr)
    , m_lblDineroCirculante(nullptr)
    , m_lblNumPrestamos(nullptr)
    , m_lblResultadoPeriodo(nullptr)
    , m_spinDias(nullptr)
{
    ui->setupUi(this);
    
    setWindowTitle("Sistema de Gestión de Préstamos Bancarios");
    setGeometry(100, 100, 1000, 600);
    
    inicializarUI();
    configurarBaseDatos();
    cargarPrestamos();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inicializarUI()
{
    // Widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(centralWidget);
    
    // Tab widget
    QTabWidget *tabWidget = new QTabWidget();
    
    // TAB 1: Gestión de Préstamos
    QWidget *tabGestion = new QWidget();
    QVBoxLayout *layoutGestion = new QVBoxLayout(tabGestion);
    
    // Tabla de préstamos
    m_tablaPrestamos = new QTableWidget();
    m_tablaPrestamos->setColumnCount(6);
    m_tablaPrestamos->setHorizontalHeaderLabels({"Cliente", "C.I.", "Monto", "Días", "Interés %", "Monto Total"});
    m_tablaPrestamos->horizontalHeader()->setStretchLastSection(true);
    m_tablaPrestamos->setAlternatingRowColors(true);
    layoutGestion->addWidget(new QLabel("Listado de Préstamos:"));
    layoutGestion->addWidget(m_tablaPrestamos);
    
    // Botones
    QHBoxLayout *layoutBotones = new QHBoxLayout();
    QPushButton *btnNuevo = new QPushButton("Nuevo Préstamo");
    QPushButton *btnEliminar = new QPushButton("Eliminar Préstamo");
    QPushButton *btnActualizar = new QPushButton("Actualizar");
    
    layoutBotones->addWidget(btnNuevo);
    layoutBotones->addWidget(btnEliminar);
    layoutBotones->addWidget(btnActualizar);
    layoutGestion->addLayout(layoutBotones);
    
    tabWidget->addTab(tabGestion, "Gestión de Préstamos");
    
    // TAB 2: Estadísticas
    QWidget *tabEstadisticas = new QWidget();
    QVBoxLayout *layoutEstadisticas = new QVBoxLayout(tabEstadisticas);
    
    QGroupBox *groupEstadisticas = new QGroupBox("Indicadores Financieros");
    QVBoxLayout *layoutGroup = new QVBoxLayout(groupEstadisticas);
    
    m_lblMontoTotal = new QLabel("Monto Total a Recuperar: $0.00");
    m_lblDineroCirculante = new QLabel("Dinero Circulante: $0.00");
    m_lblNumPrestamos = new QLabel("Total de Préstamos: 0");
    
    layoutGroup->addWidget(m_lblMontoTotal);
    layoutGroup->addWidget(m_lblDineroCirculante);
    layoutGroup->addWidget(m_lblNumPrestamos);
    
    layoutEstadisticas->addWidget(groupEstadisticas);
    layoutEstadisticas->addStretch();
    
    QPushButton *btnConsultarEstadisticas = new QPushButton("Cargar Estadísticas");
    layoutEstadisticas->addWidget(btnConsultarEstadisticas);
    
    tabWidget->addTab(tabEstadisticas, "Estadísticas");
    
    // TAB 3: Consultas por período
    QWidget *tabPeriodo = new QWidget();
    QVBoxLayout *layoutPeriodo = new QVBoxLayout(tabPeriodo);
    
    QHBoxLayout *layoutDias = new QHBoxLayout();
    QLabel *lblDias = new QLabel("Días del Período:");
    m_spinDias = new QSpinBox();
    m_spinDias->setMinimum(1);
    m_spinDias->setMaximum(365);
    m_spinDias->setValue(30);
    layoutDias->addWidget(lblDias);
    layoutDias->addWidget(m_spinDias);
    layoutDias->addStretch();
    
    QPushButton *btnPeriodo = new QPushButton("Consultar Monto a Recuperar");
    m_lblResultadoPeriodo = new QLabel("Resultado: $0.00");
    
    layoutPeriodo->addLayout(layoutDias);
    layoutPeriodo->addWidget(btnPeriodo);
    layoutPeriodo->addWidget(m_lblResultadoPeriodo);
    layoutPeriodo->addStretch();
    
    tabWidget->addTab(tabPeriodo, "Consultas por Período");
    
    layoutPrincipal->addWidget(tabWidget);
    
    // Conectar señales
    connect(btnNuevo, &QPushButton::clicked, this, &MainWindow::onNuevoPrestamo);
    connect(btnEliminar, &QPushButton::clicked, this, &MainWindow::onEliminarPrestamo);
    connect(btnActualizar, &QPushButton::clicked, this, &MainWindow::onActualizarTabla);
    connect(btnConsultarEstadisticas, &QPushButton::clicked, this, &MainWindow::onCargarEstadisticas);
    connect(btnPeriodo, &QPushButton::clicked, this, &MainWindow::onConsultarPorPeriodo);
}

void MainWindow::configurarBaseDatos()
{
    // Configurar conexión MySQL
    if (!m_gestionBanco->conectarBaseDatos("localhost", "root", "password", "banco", 3306)) {
        mostrarMensaje("Error de Conexión", 
                      "No se pudo conectar con la base de datos MySQL.\n" + 
                      m_gestionBanco->ultimoError(), true);
        return;
    }
    
    // Inicializar tabla
    if (!m_gestionBanco->inicializarBaseDatos()) {
        mostrarMensaje("Error de Inicialización", 
                      "No se pudo inicializar la base de datos.\n" + 
                      m_gestionBanco->ultimoError(), true);
    }
}

void MainWindow::cargarPrestamos()
{
    if (m_gestionBanco->cargarPrestamosDesdeDB()) {
        actualizarTablaPrestamos();
    } else {
        mostrarMensaje("Aviso", "No hay préstamos guardados o error al cargar.", false);
    }
}

void MainWindow::actualizarTablaPrestamos()
{
    if (!m_tablaPrestamos) return;
    
    m_tablaPrestamos->setRowCount(0);
    
    QList<Prestamo *> prestamos = m_gestionBanco->prestamos();
    
    for (int i = 0; i < prestamos.size(); ++i) {
        m_tablaPrestamos->insertRow(i);
        
        Prestamo *p = prestamos[i];
        m_tablaPrestamos->setItem(i, 0, new QTableWidgetItem(p->cliente()));
        m_tablaPrestamos->setItem(i, 1, new QTableWidgetItem(p->ci()));
        m_tablaPrestamos->setItem(i, 2, new QTableWidgetItem("$" + QString::number(p->monto(), 'f', 2)));
        m_tablaPrestamos->setItem(i, 3, new QTableWidgetItem(QString::number(p->dias())));
        m_tablaPrestamos->setItem(i, 4, new QTableWidgetItem(QString::number(p->porcentajeInteres(), 'f', 2) + "%"));
        m_tablaPrestamos->setItem(i, 5, new QTableWidgetItem("$" + QString::number(p->calcularMontoTotal(), 'f', 2)));
    }
}

void MainWindow::onNuevoPrestamo()
{
    // Diálogo para ingresar datos del préstamo
    bool ok;
    QString cliente = QInputDialog::getText(this, "Nuevo Préstamo", "Nombre del cliente:", QLineEdit::Normal, "", &ok);
    if (!ok || cliente.isEmpty()) return;
    
    QString ci = QInputDialog::getText(this, "Nuevo Préstamo", "Carnet de Identidad:", QLineEdit::Normal, "", &ok);
    if (!ok || ci.isEmpty()) return;
    
    double monto = QInputDialog::getDouble(this, "Nuevo Préstamo", "Monto solicitado ($):", 0, 0, 10000, 2, &ok);
    if (!ok) return;
    
    int dias = QInputDialog::getInt(this, "Nuevo Préstamo", "Días para devolución:", 30, 1, 365, 1, &ok);
    if (!ok) return;
    
    double interes = QInputDialog::getDouble(this, "Nuevo Préstamo", "Porcentaje de interés (%):", 5, 0, 100, 2, &ok);
    if (!ok) return;
    
    // Validar nuevo préstamo
    if (!m_gestionBanco->validarNuevoPrestamo(ci, monto)) {
        mostrarMensaje("Error de Validación", m_gestionBanco->ultimoError(), true);
        return;
    }
    
    // Crear préstamo
    Prestamo *nuevoPrestamo = new Prestamo(cliente, ci, monto, dias, interes, m_gestionBanco.get());
    
    // Aplicar reglas de interés
    m_gestionBanco->aplicarReglasDeInteres();
    
    // Registrar en memoria
    m_gestionBanco->registrarPrestamo(nuevoPrestamo);
    
    // Guardar en base de datos
    if (!m_gestionBanco->guardarPrestamoDB(nuevoPrestamo)) {
        mostrarMensaje("Error de Base de Datos", m_gestionBanco->ultimoError(), true);
        return;
    }
    
    mostrarMensaje("Éxito", "Préstamo registrado correctamente");
    actualizarTablaPrestamos();
}

void MainWindow::onEliminarPrestamo()
{
    bool ok;
    QString ci = QInputDialog::getText(this, "Eliminar Préstamo", "Ingrese el C.I. del cliente:", QLineEdit::Normal, "", &ok);
    if (!ok || ci.isEmpty()) return;
    
    if (!m_gestionBanco->eliminarPrestamoDelDB(ci)) {
        mostrarMensaje("Error", "No se encontró el préstamo o error al eliminar.", true);
        return;
    }
    
    mostrarMensaje("Éxito", "Préstamo eliminado correctamente");
    actualizarTablaPrestamos();
}

void MainWindow::onActualizarTabla()
{
    actualizarTablaPrestamos();
    mostrarMensaje("Información", "Tabla actualizada");
}

void MainWindow::onCargarEstadisticas()
{
    if (!m_lblMontoTotal || !m_lblDineroCirculante || !m_lblNumPrestamos) return;
    
    m_lblMontoTotal->setText("Monto Total a Recuperar: $" + QString::number(m_gestionBanco->calcularMontoTotalGeneral(), 'f', 2));
    m_lblDineroCirculante->setText("Dinero Circulante: $" + QString::number(m_gestionBanco->calcularDineroCirculante(), 'f', 2));
    m_lblNumPrestamos->setText("Total de Préstamos: " + QString::number(m_gestionBanco->prestamos().size()));
}

void MainWindow::onConsultarPorPeriodo()
{
    if (!m_spinDias || !m_lblResultadoPeriodo) return;
    
    double monto = m_gestionBanco->calcularMontoTotalARecuperar(m_spinDias->value());
    m_lblResultadoPeriodo->setText("Resultado para " + QString::number(m_spinDias->value()) + " días: $" + QString::number(monto, 'f', 2));
}

void MainWindow::mostrarMensaje(const QString &titulo, const QString &mensaje, bool esError)
{
    if (esError) {
        QMessageBox::critical(this, titulo, mensaje);
    } else {
        QMessageBox::information(this, titulo, mensaje);
    }
}

