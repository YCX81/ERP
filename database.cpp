#include "database.h"
#include <QSqlError>  // 添加这一行，包含 QSqlError 头文件
#include <QMessageBox> // 添加这一行，包含 QMessageBox 头文件

QSqlDatabase db;

bool connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("erp_system");
    db.setUserName("root");  // 替换为你的MySQL用户名
    db.setPassword("108002");  // 替换为你的MySQL密码

    if (!db.open()) {
        QMessageBox::critical(nullptr, "数据库连接错误", db.lastError().text());
        return false;
    }

    return true;
}
