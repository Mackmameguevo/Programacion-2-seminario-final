-- Script de creación de base de datos para el proyecto de Gestión de Préstamos

CREATE DATABASE IF NOT EXISTS banco CHARACTER SET utf8mb4 COLLATE utf8mb4_spanish_ci;
USE banco;

CREATE TABLE IF NOT EXISTS Prestamos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    cliente VARCHAR(100) NOT NULL,
    ci VARCHAR(30) NOT NULL UNIQUE,
    monto DOUBLE NOT NULL,
    dias INT NOT NULL,
    porcentaje_interes DOUBLE NOT NULL,
    monto_total DOUBLE NOT NULL,
    estado VARCHAR(50) NOT NULL DEFAULT 'Activo',
    fecha_creacion TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

