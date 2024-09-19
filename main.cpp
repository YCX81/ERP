#include "mainwindow.h"
#include "main.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    if (!connectToDatabase()) {
        return -1; // 如果数据库连接失败，退出程序
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

    if (!db.open()) {
        QMessageBox::critical(nullptr, "数据库连接失败", db.lastError().text());
        return false;
    }
    return true;
}
