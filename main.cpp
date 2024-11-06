#include "mainwindow.h"
#include "main.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QUndoStack>

bool testDatabase() {
    QSqlQuery query;
    query.prepare("INSERT INTO Users (username, password_hash) VALUES ('testuser', 'testhash')");

    if (!query.exec()) {
        qDebug() << "插入数据失败: " << query.lastError().text();
        return false;
    }

    query.prepare("SELECT * FROM Users");
    if (!query.exec()) {
        qDebug() << "查询数据失败: " << query.lastError().text();
        return false;
    }

    while (query.next()) {
        QString username = query.value("username").toString();
        QString password_hash = query.value("password_hash").toString();
        qDebug() << "用户名: " << username << "，密码哈希: " << password_hash;
    }

    return true;
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
    if (!connectToDatabase()) {
        return -1; // 如果数据库连接失败，退出程序
    }
    if (!testDatabase()) {
        qDebug() << "数据库测试失败！";
    } else {
        qDebug() << "数据库测试成功！";
    }

    MainWindow w;
    w.show();
    return app.exec();
}


bool connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("erp_system");
    db.setUserName("root");
    db.setPassword("108002");
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1;CHARSET=utf8mb4");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "数据库连接失败", db.lastError().text());
        return false;
    }
    qDebug() << "数据库连接成功！";
    return true;
}
