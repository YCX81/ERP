#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materialdialog.h"
#include "database.h"
#include <QMessageBox>
#include <QSqlDatabase>

// 构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , materialDialog(new MaterialDialog(this))
{
    ui->setupUi(this);

    // 连接到数据库
    if (!Database::instance().connectToDatabase("localhost", "ERP_database", "root", "108002")) {
        QMessageBox::critical(this, "数据库错误", "无法连接到数据库。");
    }

    connect(ui->addMaterialButton, &QPushButton::clicked, this, &MainWindow::on_addMaterialButton_clicked);
    connect(ui->editMaterialButton, &QPushButton::clicked, this, &MainWindow::on_editMaterialButton_clicked);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::on_undoButton_clicked);

    loadMaterials();
}

// 添加析构函数的实现
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");  // 使用适当的数据库驱动
    db.setHostName("localhost");
    db.setDatabaseName("erp_system");
    db.setUserName("root");
    db.setPassword("108002");

    if (!db.open()) {
        // 显示错误信息
    } else {
        setupMaterialModel(db);  // 调用初始化函数
    }
}

void MainWindow::loadMaterials()
{
    // 清空表格内容
    ui->materialTable->setRowCount(0);

    QSqlQuery query = Database::instance().getMaterials();
    while (query.next()) {
        int row = ui->materialTable->rowCount();
        ui->materialTable->insertRow(row);
        ui->materialTable->setItem(row, 0, new QTableWidgetItem(query.value("name").toString()));
        ui->materialTable->setItem(row, 1, new QTableWidgetItem(query.value("type").toString()));
        ui->materialTable->setItem(row, 2, new QTableWidgetItem(query.value("quantity").toString()));
        ui->materialTable->setItem(row, 3, new QTableWidgetItem(query.value("price").toString()));
    }
}

void MainWindow::on_addMaterialButton_clicked()
{
    materialDialog->setMaterialData("", "", 0, 0.0);
    if (materialDialog->exec() == QDialog::Accepted) {
        QString name = materialDialog->getName();
        QString type = materialDialog->getType();
        int quantity = materialDialog->getQuantity();
        double price = materialDialog->getPrice();
        if (Database::instance().addMaterial(name, type, quantity, price)) {
            loadMaterials();
        } else {
            QMessageBox::warning(this, "错误", "无法添加物料到数据库。");
        }
    }
}

void MainWindow::on_editMaterialButton_clicked()
{
    int row = ui->materialTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "警告", "请选择一个物料进行编辑");
        return;
    }

    int id = ui->materialTable->item(row, 0)->data(Qt::UserRole).toInt();
    QString name = ui->materialTable->item(row, 0)->text();
    QString type = ui->materialTable->item(row, 1)->text();
    int quantity = ui->materialTable->item(row, 2)->text().toInt();
    double price = ui->materialTable->item(row, 3)->text().toDouble();

    materialDialog->setMaterialData(name, type, quantity, price);

    if (materialDialog->exec() == QDialog::Accepted) {
        name = materialDialog->getName();
        type = materialDialog->getType();
        quantity = materialDialog->getQuantity();
        price = materialDialog->getPrice();
        if (Database::instance().updateMaterial(id, name, type, quantity, price)) {
            loadMaterials();
        } else {
            QMessageBox::warning(this, "错误", "无法更新数据库中的物料信息。");
        }
    }
}

void MainWindow::on_undoButton_clicked()
{
    // 实现撤销逻辑
    if (!undoStack.isEmpty()) {
        QVariant lastAction = undoStack.pop();
        // 恢复代码逻辑
    }
}
