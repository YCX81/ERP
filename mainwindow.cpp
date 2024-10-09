// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerdialog.h"
#include "logindialog.h"
#include "materialdialog.h"
#include "defectivedialog.h"
#include "bomdialog.h"
#include "orderdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlTableModel>
#include <QTabWidget>
#include <QDateTime>
#include <xlsxdocument.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , materialModel(new QSqlTableModel(this))
    , defectiveModel(new QSqlTableModel(this))
    , bomModel(new QSqlTableModel(this))
    , supplierModel(new QSqlTableModel(this))
{
    ui->setupUi(this);

    // 初始化物料模型
    materialModel->setTable("Material");
    materialModel->select();
    ui->materialTableView->setModel(materialModel);

    // 设置表头名称
    materialModel->setHeaderData(materialModel->fieldIndex("id"), Qt::Horizontal, "ID");
    materialModel->setHeaderData(materialModel->fieldIndex("material_number"), Qt::Horizontal, "物料号");
    materialModel->setHeaderData(materialModel->fieldIndex("category"), Qt::Horizontal, "类别");
    materialModel->setHeaderData(materialModel->fieldIndex("description"), Qt::Horizontal, "描述");
    materialModel->setHeaderData(materialModel->fieldIndex("quantity"), Qt::Horizontal, "数量");
    materialModel->setHeaderData(materialModel->fieldIndex("unit_price"), Qt::Horizontal, "单价");
    materialModel->setHeaderData(materialModel->fieldIndex("self_made_or_purchase"), Qt::Horizontal, "来源");
    materialModel->setHeaderData(materialModel->fieldIndex("version"), Qt::Horizontal, "版本号");
    materialModel->setHeaderData(materialModel->fieldIndex("serial_number"), Qt::Horizontal, "序列号");
    materialModel->setHeaderData(materialModel->fieldIndex("location_number"), Qt::Horizontal, "库位号");
    materialModel->setHeaderData(materialModel->fieldIndex("good_quantity"), Qt::Horizontal, "良品数量");
    materialModel->setHeaderData(materialModel->fieldIndex("defective_quantity"), Qt::Horizontal, "不良品数量");
    materialModel->setHeaderData(materialModel->fieldIndex("supplier"), Qt::Horizontal, "供应商");
    materialModel->setHeaderData(materialModel->fieldIndex("supplier_material_number"), Qt::Horizontal, "供应商料号");
    materialModel->setHeaderData(materialModel->fieldIndex("delivery_period"), Qt::Horizontal, "货期");
    materialModel->setHeaderData(materialModel->fieldIndex("remarks"), Qt::Horizontal, "备注");
    materialModel->setHeaderData(materialModel->fieldIndex("material_maintainer"), Qt::Horizontal, "物料维护人");
    materialModel->setHeaderData(materialModel->fieldIndex("update_date"), Qt::Horizontal, "更新日期");

    // 隐藏 ID 列（如果不需要显示）
    ui->materialTableView->setColumnHidden(materialModel->fieldIndex("id"), true);

    // 初始化不良品模型，只显示不良品数量大于0的记录
    defectiveModel->setTable("Material");
    defectiveModel->setFilter("defective_quantity > 0");
    defectiveModel->select();
    ui->defectiveTableView->setModel(defectiveModel);

    // 初始化 BOM 模型
    bomModel->setTable("BOM");
    bomModel->select();
    ui->bomTableView->setModel(bomModel);

    // 初始化供应商模型
    supplierModel = new QSqlTableModel(this);
    supplierModel->setTable("Suppliers");
    supplierModel->select();
    ui->supplierTableView->setModel(supplierModel);

    // 设置表头名称
    supplierModel->setHeaderData(supplierModel->fieldIndex("id"), Qt::Horizontal, "ID");
    supplierModel->setHeaderData(supplierModel->fieldIndex("supplier_id"), Qt::Horizontal, "供应商编号");
    supplierModel->setHeaderData(supplierModel->fieldIndex("supplier_name"), Qt::Horizontal, "供应商名称");

    // 隐藏 ID 列
    ui->supplierTableView->setColumnHidden(supplierModel->fieldIndex("id"), true);

    // 初始化供应商物料模型
    supplierMaterialModel = new QSqlRelationalTableModel(this);
    supplierMaterialModel->setTable("Material");
    supplierMaterialModel->setRelation(supplierMaterialModel->fieldIndex("supplier_id"), QSqlRelation("Suppliers", "supplier_id", "supplier_name"));
    supplierMaterialModel->select();
    ui->supplierMaterialTableView->setModel(supplierMaterialModel);

    // 设置表头名称
    supplierMaterialModel->setHeaderData(supplierMaterialModel->fieldIndex("material_number"), Qt::Horizontal, "物料号");
    supplierMaterialModel->setHeaderData(supplierMaterialModel->fieldIndex("description"), Qt::Horizontal, "描述");
    supplierMaterialModel->setHeaderData(supplierMaterialModel->fieldIndex("supplier_id"), Qt::Horizontal, "供应商");


    // 初始化订单模型和视图
    QStringList statuses = { "预下单", "缺料", "制作中", "制作完毕", "已出库" };
    for (const QString &status : statuses) {
        QSqlTableModel *orderModel = new QSqlTableModel(this);
        orderModel->setTable("Orders");
        orderModel->setFilter(QString("status = '%1'").arg(status));
        orderModel->select();
        orderModels[status] = orderModel;

        QTableView *tableView = new QTableView;
        tableView->setModel(orderModel);
        orderTableViews[status] = tableView;

        QWidget *tab = new QWidget;
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);
        tabLayout->addWidget(tableView);
        ui->orderTabWidget->addTab(tab, status);
    }

    // 连接信号槽
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::on_registerButton_clicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::on_loginButton_clicked);
    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);
    connect(ui->switchUserButton, &QPushButton::clicked, this, &MainWindow::on_switchUserButton_clicked);

    connect(ui->materialButton, &QPushButton::clicked, this, &MainWindow::on_materialButton_clicked);
    connect(ui->defectiveButton, &QPushButton::clicked, this, &MainWindow::on_defectiveButton_clicked);
    connect(ui->bomButton, &QPushButton::clicked, this, &MainWindow::on_bomButton_clicked);
    connect(ui->orderButton, &QPushButton::clicked, this, &MainWindow::on_orderButton_clicked);
    connect(ui->supplierButton, &QPushButton::clicked, this, &MainWindow::on_supplierButton_clicked);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButton_clicked);
    connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::on_editButton_clicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::on_exportButton_clicked);
    connect(ui->viewDrawingButton, &QPushButton::clicked, this, &MainWindow::on_viewDrawingButton_clicked);
    connect(ui->viewPhotoButton, &QPushButton::clicked, this, &MainWindow::on_viewPhotoButton_clicked);

    connect(ui->addDefectiveButton, &QPushButton::clicked, this, &MainWindow::on_addDefectiveButton_clicked);
    connect(ui->editDefectiveButton, &QPushButton::clicked, this, &MainWindow::on_editDefectiveButton_clicked);
    connect(ui->deleteDefectiveButton, &QPushButton::clicked, this, &MainWindow::on_deleteDefectiveButton_clicked);

    connect(ui->addBOMButton, &QPushButton::clicked, this, &MainWindow::on_addBOMButton_clicked);
    connect(ui->editBOMButton, &QPushButton::clicked, this, &MainWindow::on_editBOMButton_clicked);
    connect(ui->deleteBOMButton, &QPushButton::clicked, this, &MainWindow::on_deleteBOMButton_clicked);

    connect(ui->addOrderButton, &QPushButton::clicked, this, &MainWindow::on_addOrderButton_clicked);
    connect(ui->editOrderButton, &QPushButton::clicked, this, &MainWindow::on_editOrderButton_clicked);
    connect(ui->deleteOrderButton, &QPushButton::clicked, this, &MainWindow::on_deleteOrderButton_clicked);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::on_undoButton_clicked);

    connect(ui->addSupplierButton, &QPushButton::clicked, this, &MainWindow::on_addSupplierButton_clicked);
    connect(ui->editSupplierButton, &QPushButton::clicked, this, &MainWindow::on_editSupplierButton_clicked);
    connect(ui->deleteSupplierButton, &QPushButton::clicked, this, &MainWindow::on_deleteSupplierButton_clicked);

    connect(ui->materialTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onMaterialTableViewSelectionChanged);
    connect(ui->supplierTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSupplierSelectionChanged);



    // 更新登录状态
    updateLoginStatus();

    // 默认显示物料管理页面
    ui->stackedWidget->setCurrentWidget(ui->materialPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerButton_clicked()
{
    RegisterDialog regDialog(this);
    if (regDialog.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "注册成功", "用户已成功注册。");
    }
}

void MainWindow::on_loginButton_clicked()
{
    LoginDialog loginDialog(this);
    if (loginDialog.exec() == QDialog::Accepted) {
        currentUser = loginDialog.getUsername();
        updateLoginStatus();
    }
}

void MainWindow::on_logoutButton_clicked()
{
    currentUser.clear();
    updateLoginStatus();
}

void MainWindow::on_switchUserButton_clicked()
{
    on_loginButton_clicked();
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
        ui->currentUserLabel->setText("");
        ui->statusbar->showMessage(tr("未登录"));
    }
}

void MainWindow::on_materialButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->materialPage);
}

void MainWindow::on_defectiveButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->defectivePage);
}

void MainWindow::on_bomButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->bomPage);
}

void MainWindow::on_orderButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->orderPage);
}

void MainWindow::on_supplierButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->supplierPage);
}

void MainWindow::on_addButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    MaterialDialog dialog(currentUser, this);
    if (dialog.exec() == QDialog::Accepted) {
        materialModel->select();
    }
}

void MainWindow::on_editButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的物料。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = materialModel->record(row);

    MaterialDialog dialog(currentUser, this);
    dialog.setMaterialData(record);
    if (dialog.exec() == QDialog::Accepted) {
        materialModel->select();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的物料。");
        return;
    }

    int ret = QMessageBox::question(this, "确认删除", "确定要删除选中的物料吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        int row = selectedRows.first().row();
        materialModel->removeRow(row);
        if (!materialModel->submitAll()) {
            QMessageBox::critical(this, "错误", "无法删除物料：" + materialModel->lastError().text());
        } else {
            materialModel->select();
        }
    }
}

void MainWindow::on_exportButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出Excel", "", "Excel 文件 (*.xlsx)");
    if (!filePath.isEmpty()) {
        QXlsx::Document xlsxDoc;

        // 获取模型数据
        int rowCount = materialModel->rowCount();
        int columnCount = materialModel->columnCount();

        // 写入表头
        int colOffset = 1;
        for (int col = 0; col < columnCount; ++col) {
            if (ui->materialTableView->isColumnHidden(col)) {
                continue;
            }
            QString header = materialModel->headerData(col, Qt::Horizontal).toString();
            xlsxDoc.write(1, colOffset, header);
            ++colOffset;
        }

        // 写入数据
        for (int row = 0; row < rowCount; ++row) {
            colOffset = 1;
            for (int col = 0; col < columnCount; ++col) {
                if (ui->materialTableView->isColumnHidden(col)) {
                    continue;
                }
                QVariant data = materialModel->data(materialModel->index(row, col));
                xlsxDoc.write(row + 2, colOffset, data);
                ++colOffset;
            }
        }

        xlsxDoc.saveAs(filePath);
        QMessageBox::information(this, "导出成功", "数据已成功导出。");
    }
}

void MainWindow::on_viewDrawingButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    int id = materialModel->data(materialModel->index(row, materialModel->fieldIndex("id"))).toInt();

    QSqlQuery query;
    query.prepare("SELECT drawing FROM Material WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        QByteArray data = query.value(0).toByteArray();
        if (data.isEmpty()) {
            QMessageBox::information(this, "提示", "该物料没有图纸。");
            return;
        }
        displayDataInPreview(data);
    } else {
        QMessageBox::warning(this, "错误", "无法获取图纸数据：" + query.lastError().text());
    }
}

void MainWindow::on_viewPhotoButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    int id = materialModel->data(materialModel->index(row, materialModel->fieldIndex("id"))).toInt();

    QSqlQuery query;
    query.prepare("SELECT photo FROM Material WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        QByteArray data = query.value(0).toByteArray();
        if (data.isEmpty()) {
            QMessageBox::information(this, "提示", "该物料没有照片。");
            return;
        }
        displayDataInPreview(data);
    } else {
        QMessageBox::warning(this, "错误", "无法获取照片数据：" + query.lastError().text());
    }
}

void MainWindow::displayDataInPreview(const QByteArray &data)
{
    QPixmap pixmap;
    if (pixmap.loadFromData(data)) {
        ui->previewLabel->setPixmap(pixmap.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QMessageBox::warning(this, "警告", "无法显示数据。");
    }
}

void MainWindow::on_addDefectiveButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = materialModel->record(row);

    MaterialDialog dialog(currentUser, this);
    dialog.setMaterialData(record);
    dialog.enableDefectiveQuantityEditing(true);
    if (dialog.exec() == QDialog::Accepted) {
        defectiveModel->select();
        materialModel->select(); // 更新物料表
    }
}

void MainWindow::on_editDefectiveButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->defectiveTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的不良品。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = defectiveModel->record(row);

    MaterialDialog dialog(currentUser, this);
    dialog.setMaterialData(record);
    dialog.enableDefectiveQuantityEditing(true);
    if (dialog.exec() == QDialog::Accepted) {
        defectiveModel->select();
        materialModel->select(); // 更新物料表
    }
}

void MainWindow::on_deleteDefectiveButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->defectiveTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的不良品。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = defectiveModel->record(row);

    int ret = QMessageBox::question(this, "确认删除", "确定要将该物料的不良品数量清零吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        // 将 defective_quantity 设为 0
        QSqlQuery query;
        query.prepare("UPDATE Material SET defective_quantity = 0 WHERE id = :id");
        query.bindValue(":id", record.value("id"));
        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "无法删除不良品：" + query.lastError().text());
        } else {
            defectiveModel->select();
            materialModel->select(); // 更新物料表
        }
    }
}

void MainWindow::on_addBOMButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    BOMDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        bomModel->select();
    }
}

void MainWindow::on_editBOMButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->bomTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的 BOM。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = bomModel->record(row);

    BOMDialog dialog(this);
    dialog.setBOMData(record);
    if (dialog.exec() == QDialog::Accepted) {
        bomModel->select();
    }
}

void MainWindow::on_deleteBOMButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->bomTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的 BOM。");
        return;
    }

    int row = selectedRows.first().row();
    int bomId = bomModel->record(row).value("id").toInt();

    int ret = QMessageBox::question(this, "确认删除", "确定要删除该 BOM 及其物料清单吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        // 删除 BOM_Material 中的相关记录
        QSqlQuery query;
        query.prepare("DELETE FROM BOM_Material WHERE bom_id = :bom_id");
        query.bindValue(":bom_id", bomId);
        if (!query.exec()) {
            QMessageBox::warning(this, "警告", "无法删除 BOM 物料清单：" + query.lastError().text());
        }

        // 删除 BOM
        bomModel->removeRow(row);
        if (!bomModel->submitAll()) {
            QMessageBox::critical(this, "错误", "无法删除 BOM：" + bomModel->lastError().text());
        } else {
            bomModel->select();
        }
    }
}

void MainWindow::on_addOrderButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    OrderDialog dialog(currentUser, this);

    if (dialog.exec() == QDialog::Accepted) {
        // 刷新所有订单模型
        foreach (QSqlTableModel *model, orderModels) {
            model->select();
        }
    }
}

void MainWindow::on_editOrderButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    int currentIndex = ui->orderTabWidget->currentIndex();
    QString currentStatus = ui->orderTabWidget->tabText(currentIndex);

    QTableView *currentTableView = orderTableViews[currentStatus];
    QModelIndexList selectedRows = currentTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的订单。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = orderModels[currentStatus]->record(row);

    OrderDialog dialog(currentUser, this);
    dialog.setOrderData(record);
    if (dialog.exec() == QDialog::Accepted) {
        // 刷新所有订单模型
        foreach (QSqlTableModel *model, orderModels) {
            model->select();
        }
    }
}

void MainWindow::on_deleteOrderButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    int currentIndex = ui->orderTabWidget->currentIndex();
    QString currentStatus = ui->orderTabWidget->tabText(currentIndex);

    QTableView *currentTableView = orderTableViews[currentStatus];
    QModelIndexList selectedRows = currentTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的订单。");
        return;
    }

    int row = selectedRows.first().row();
    int ret = QMessageBox::question(this, "确认删除", "确定要删除选中的订单吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        orderModels[currentStatus]->removeRow(row);
        if (!orderModels[currentStatus]->submitAll()) {
            QMessageBox::critical(this, "错误", "无法删除订单：" + orderModels[currentStatus]->lastError().text());
        } else {
            // 刷新所有订单模型
            foreach (QSqlTableModel *model, orderModels) {
                model->select();
            }
        }
    }
}

void MainWindow::on_undoButton_clicked()
{
    // 实现撤销功能（需要维护操作记录）
}

void MainWindow::on_addSupplierButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    // 实现添加供应商的功能
    // SupplierDialog dialog(this);
    // if (dialog.exec() == QDialog::Accepted) {
    //     supplierModel->select();
    // }
}

void MainWindow::on_editSupplierButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->supplierTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的供应商。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = supplierModel->record(row);

    // SupplierDialog dialog(this);
    // dialog.setSupplierData(record);
    // if (dialog.exec() == QDialog::Accepted) {
    //     supplierModel->select();
    // }
}

void MainWindow::on_deleteSupplierButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->supplierTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的供应商。");
        return;
    }

    int row = selectedRows.first().row();
    int ret = QMessageBox::question(this, "确认删除", "确定要删除选中的供应商吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        supplierModel->removeRow(row);
        if (!supplierModel->submitAll()) {
            QMessageBox::critical(this, "错误", "无法删除供应商：" + supplierModel->lastError().text());
        } else {
            supplierModel->select();
        }
    }
}

void MainWindow::onSupplierSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndexList selectedRows = ui->supplierTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        int row = selectedRows.first().row();
        QString supplierId = supplierModel->data(supplierModel->index(row, supplierModel->fieldIndex("supplier_id"))).toString();

        // 过滤物料表，显示选定供应商的物料
        supplierMaterialModel->setFilter(QString("supplier_id = '%1'").arg(supplierId));
        supplierMaterialModel->select();
    } else {
        // 如果没有选择供应商，显示所有物料或清空
        supplierMaterialModel->setFilter("");
        supplierMaterialModel->select();
    }
}

void MainWindow::onMaterialTableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        int row = selectedRows.first().row();
        int id = materialModel->data(materialModel->index(row, materialModel->fieldIndex("id"))).toInt();

        // 加载实物照片进行预览
        QSqlQuery query;
        query.prepare("SELECT photo FROM Material WHERE id = :id");
        query.bindValue(":id", id);
        if (query.exec() && query.next()) {
            QByteArray data = query.value(0).toByteArray();
            if (!data.isEmpty()) {
                displayDataInPreview(data);
            } else {
                ui->previewLabel->clear();
                ui->previewLabel->setText("无预览图像");
            }
        }
    } else {
        ui->previewLabel->clear();
        ui->previewLabel->setText("无预览图像");
    }
}

void MainWindow::on_orderStatusChanged(int orderId, const QString &newStatus)
{
    QSqlQuery query;
    query.prepare("SELECT status FROM `Order` WHERE id = :id");
    query.bindValue(":id", orderId);
    if (query.exec() && query.next()) {
        QString oldStatus = query.value("status").toString();

        // 更新订单状态
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE `Order` SET status = :status, updated_at = NOW() WHERE id = :id");
        updateQuery.bindValue(":status", newStatus);
        updateQuery.bindValue(":id", orderId);
        if (!updateQuery.exec()) {
            QMessageBox::warning(this, "警告", "无法更新订单状态：" + updateQuery.lastError().text());
            return;
        }

        // 根据状态变化处理库存
        if (oldStatus == "预下单" && newStatus == "制作中") {
            // 扣减物料库存
            processMaterialInventory(orderId, "deduct");
        } else if (oldStatus == "制作中" && newStatus == "制作完毕") {
            // 增加成品库存
            processProductInventory(orderId, "increase");
        } else if (oldStatus == "制作完毕" && newStatus == "已出库") {
            // 扣减成品库存
            processProductInventory(orderId, "deduct");
        }

        // 添加订单操作日志
        QSqlQuery logQuery;
        logQuery.prepare("INSERT INTO OrderLog (order_id, operation_type, operator, operation_date, details) "
                         "VALUES (:order_id, 'UPDATE', :operator, NOW(), :details)");
        logQuery.bindValue(":order_id", orderId);
        logQuery.bindValue(":operator", currentUser);
        logQuery.bindValue(":details", QString("订单状态从 '%1' 变为 '%2'").arg(oldStatus).arg(newStatus));
        logQuery.exec();
    }
}

void MainWindow::processMaterialInventory(int orderId, const QString &operation)
{
    // 扣减或增加物料库存
    QSqlQuery productQuery;
    productQuery.prepare("SELECT product_id, quantity FROM Order_Product WHERE order_id = :order_id");
    productQuery.bindValue(":order_id", orderId);
    if (productQuery.exec()) {
        while (productQuery.next()) {
            int productId = productQuery.value("product_id").toInt();
            int productQuantity = productQuery.value("quantity").toInt();

            // 查询成品对应的 BOM
            QSqlQuery bomQuery;
            bomQuery.prepare("SELECT material_id, quantity FROM BOM_Material WHERE bom_id = :bom_id");
            bomQuery.bindValue(":bom_id", productId);
            if (bomQuery.exec()) {
                while (bomQuery.next()) {
                    int materialId = bomQuery.value("material_id").toInt();
                    int materialQuantityRequired = bomQuery.value("quantity").toInt() * productQuantity;

                    // 更新物料库存
                    QSqlQuery materialQuery;
                    QString sql = (operation == "deduct") ? "UPDATE Material SET good_quantity = good_quantity - :quantity WHERE id = :id"
                                                          : "UPDATE Material SET good_quantity = good_quantity + :quantity WHERE id = :id";
                    materialQuery.prepare(sql);
                    materialQuery.bindValue(":quantity", materialQuantityRequired);
                    materialQuery.bindValue(":id", materialId);
                    if (!materialQuery.exec()) {
                        QMessageBox::warning(this, "警告", "无法更新物料库存：" + materialQuery.lastError().text());
                    } else {
                        // 添加物料操作日志
                        QSqlQuery logQuery;
                        logQuery.prepare("INSERT INTO MaterialLog (material_id, operation_type, operator, operation_date, details) "
                                         "VALUES (:material_id, 'UPDATE', :operator, NOW(), :details)");
                        logQuery.bindValue(":material_id", materialId);
                        logQuery.bindValue(":operator", currentUser);
                        QString detail = (operation == "deduct") ? QString("物料扣减 %1").arg(materialQuantityRequired)
                                                                 : QString("物料增加 %1").arg(materialQuantityRequired);
                        logQuery.bindValue(":details", detail);
                        logQuery.exec();
                    }
                }
            }
        }
    }
}

void MainWindow::processProductInventory(int orderId, const QString &operation)
{
    // 扣减或增加成品库存
    QSqlQuery productQuery;
    productQuery.prepare("SELECT product_id, quantity FROM Order_Product WHERE order_id = :order_id");
    productQuery.bindValue(":order_id", orderId);
    if (productQuery.exec()) {
        while (productQuery.next()) {
            int productId = productQuery.value("product_id").toInt();
            int productQuantity = productQuery.value("quantity").toInt();

            // 更新成品库存（假设成品也在 Material 表中）
            QSqlQuery productUpdateQuery;
            QString sql = (operation == "deduct") ? "UPDATE Material SET good_quantity = good_quantity - :quantity WHERE id = :id"
                                                  : "UPDATE Material SET good_quantity = good_quantity + :quantity WHERE id = :id";
            productUpdateQuery.prepare(sql);
            productUpdateQuery.bindValue(":quantity", productQuantity);
            productUpdateQuery.bindValue(":id", productId);
            if (!productUpdateQuery.exec()) {
                QMessageBox::warning(this, "警告", "无法更新成品库存：" + productUpdateQuery.lastError().text());
            } else {
                // 添加物料操作日志
                QSqlQuery logQuery;
                logQuery.prepare("INSERT INTO MaterialLog (material_id, operation_type, operator, operation_date, details) "
                                 "VALUES (:material_id, 'UPDATE', :operator, NOW(), :details)");
                logQuery.bindValue(":material_id", productId);
                logQuery.bindValue(":operator", currentUser);
                QString detail = (operation == "deduct") ? QString("成品库存扣减 %1").arg(productQuantity)
                                                         : QString("成品库存增加 %1").arg(productQuantity);
                logQuery.bindValue(":details", detail);
                logQuery.exec();
            }
        }
    }
}
