#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerdialog.h"  // 假设有一个注册对话框
#include "logindialog.h"  // 假设有一个登录对话框
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <xlsxdocument.h>
  // 假设你使用QXlsx库

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new QSqlTableModel(this))
{
    ui->setupUi(this);

    // 初始化模型
    model->setTable("materials");
    model->select();
    ui->materialTableView->setModel(model);

    // 初始化状态
    ui->registerButton->setVisible(true);
    ui->loginButton->setVisible(true);
    ui->logoutButton->setVisible(false);
    ui->switchUserButton->setVisible(false);
    ui->currentUserLabel->setVisible(false);

    // 手动连接 QTableView 的 selectionModel 的 selectionChanged 信号到槽函数
    connect(ui->materialTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onMaterialTableViewSelectionChanged);

    updateLoginStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    MaterialDialog dialog(this);
    setMaintainer();  // 设置当前用户为物料维护人
    if (dialog.exec() == QDialog::Accepted) {
        QSqlRecord newRecord = dialog.getMaterialData();
        model->insertRecord(-1, newRecord);
        if (!model->submitAll()) {
            QMessageBox::critical(this, "错误", "无法保存物料信息：" + model->lastError().text());
        }
        refreshMaterialTable();
    }
}

void MainWindow::on_editButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的物料。");
        return;
    }

    int row = selectedRows.first().row();
    MaterialDialog dialog(this);
    dialog.setMaterialData(model->record(row));
    if (dialog.exec() == QDialog::Accepted) {
        model->setRecord(row, dialog.getMaterialData());
        if (!model->submitAll()) {
            QMessageBox::critical(this, "错误", "无法保存物料信息：" + model->lastError().text());
        }
        refreshMaterialTable();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的物料。");
        return;
    }

    int row = selectedRows.first().row();
    model->removeRow(row);
    if (!model->submitAll()) {
        QMessageBox::critical(this, "错误", "无法删除物料：" + model->lastError().text());
    }
    refreshMaterialTable();
}

void MainWindow::on_exportButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出Excel", "", "Excel 文件 (*.xlsx)");
    if (!filePath.isEmpty()) {
        QXlsx::Document xlsxDoc;
        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                xlsxDoc.write(row + 1, col + 1, model->record(row).value(col));
            }
        }
        xlsxDoc.saveAs(filePath);
        QMessageBox::information(this, "导出成功", "数据已成功导出。");
    }
}

void MainWindow::on_loginButton_clicked()
{
    LoginDialog loginDialog(this);  // 假设有一个登录对话框
    if (loginDialog.exec() == QDialog::Accepted) {
        currentUser = loginDialog.getUsername();  // 从登录对话框获取用户名
        updateLoginStatus();
    }
}

void MainWindow::on_registerButton_clicked()
{
    RegisterDialog regDialog(this);  // 假设有一个注册对话框
    if (regDialog.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "注册成功", "用户已成功注册。");
    }
}

void MainWindow::on_logoutButton_clicked()
{
    currentUser.clear();
    updateLoginStatus();
}

void MainWindow::on_switchUserButton_clicked()
{
    on_loginButton_clicked();  // 切换用户时重新执行登录逻辑
}

void MainWindow::onMaterialTableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // 根据选中的物料更新预览框内容
    QModelIndexList indexes = selected.indexes();
    if (!indexes.isEmpty()) {
        // 例如，显示选中的物料信息
        QModelIndex selectedIndex = indexes.first();
        QSqlRecord record = model->record(selectedIndex.row());
        qDebug() << "Selected material: " << record.value("model").toString();
    }
}

void MainWindow::updateLoginStatus()
{
    bool isLoggedIn = !currentUser.isEmpty();
    ui->loginButton->setVisible(!isLoggedIn);
    ui->registerButton->setVisible(!isLoggedIn);
    ui->logoutButton->setVisible(isLoggedIn);
    ui->switchUserButton->setVisible(isLoggedIn);
    ui->currentUserLabel->setVisible(isLoggedIn);
    if (isLoggedIn) {
        ui->currentUserLabel->setText(tr("当前用户：%1").arg(currentUser));
        ui->statusbar->showMessage(tr("当前用户：%1").arg(currentUser));
    } else {
        ui->statusbar->showMessage(tr("未登录"));
    }
}

void MainWindow::refreshMaterialTable()
{
    model->select();  // 重新从数据库获取数据
    ui->materialTableView->reset();  // 刷新表格视图
}

void MainWindow::setMaintainer()
{
    // 将物料维护人设置为当前用户
    if (!currentUser.isEmpty()) {
        //materialDialog->setCurrentMaintainer(currentUser);  // 假设在MaterialDialog中有相应的函数
    }
}
