-- 创建数据库
CREATE DATABASE erp_system;

-- 使用数据库
USE erp_system;

-- 物料表
CREATE TABLE Material (
                          id INT AUTO_INCREMENT PRIMARY KEY,
                          model VARCHAR(255) NOT NULL,
                          description TEXT,
                          quantity INT DEFAULT 0,
                          unit_price DECIMAL(10, 2) DEFAULT 0.0,
                          self_made_or_purchase CHAR(1) CHECK (self_made_or_purchase IN ('P', 'M')),
                          drawing LONGBLOB, -- 存储图纸数据
                          photo LONGBLOB, -- 存储实物照片数据
                          version VARCHAR(50),
                          serial_number VARCHAR(255),
                          location_number VARCHAR(255),
                          status CHAR(2) CHECK (status IN ('PP', 'FP')),
                          supplier VARCHAR(255),
                          supplier_material_number VARCHAR(255),
                          supplier_specification LONGBLOB, -- 存储供应商规格书数据
                          delivery_period DATE,
                          material_maintainer VARCHAR(255),
                          update_date DATETIME DEFAULT CURRENT_TIMESTAMP,
                          remarks TEXT
);

-- 日志表
CREATE TABLE MaterialLog (
                             id INT AUTO_INCREMENT PRIMARY KEY,
                             material_id INT,
                             operation_type ENUM('CREATE', 'UPDATE', 'DELETE'),
                             operation_date DATETIME DEFAULT CURRENT_TIMESTAMP,
                             operator VARCHAR(255),
                             details TEXT,
                             FOREIGN KEY (material_id) REFERENCES Material(id)
);

-- 用户表
CREATE TABLE Users (
                       id INT AUTO_INCREMENT PRIMARY KEY,
                       username VARCHAR(50) UNIQUE NOT NULL,
                       password_hash VARCHAR(255) NOT NULL,
                       created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 检查表结构
DESCRIBE Users;
