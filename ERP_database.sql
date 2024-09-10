CREATE DATABASE erp_system;

USE erp_system;

-- 用户信息表
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,  -- 存储哈希后的密码
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 物料信息表
CREATE TABLE materials (
    id INT AUTO_INCREMENT PRIMARY KEY,
    category VARCHAR(50) NOT NULL,
    description TEXT,
    drawing_path VARCHAR(255),
    supplier VARCHAR(50),
    maintainer VARCHAR(50),
    sn VARCHAR(50),
    version VARCHAR(20),
    update_date DATE,
    remarks TEXT,
    price DECIMAL(10, 2),
    currency VARCHAR(10),
    quantity INT,
    delivery_time DATE,
    photo_path VARCHAR(255),
    specification TEXT,
    location VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS materials (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT,
    category TEXT,
    quantity INTEGER,
    price REAL
);
