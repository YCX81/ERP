#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerdialog.h"
#include "logindialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <xlsxdocument.h>
#include <QPdfDocument>
#include <QPainter>
#include <QBuffer>
#include <QPixmap>
#include <QImage>
#include <QPdfDocumentRenderOptions>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new QSqlTableModel(this))
{
    ui->setupUi(this);

    // 初始化模型，只选择必要的字段，排除大字段
    model->setQuery("SELECT id, model, description, quantity, unit_price, self_made_or_purchase, version, "
                    "serial_number, location_number, status, supplier, supplier_material_number, delivery_period, "
                    "material_maintainer, update_date, remarks FROM Material");

    if (model->lastError().isValid()) {
        qDebug() << "Error in model query:" << model->lastError().text();
    }
    ui->materialTableView->setModel(model);


    // 设置表头名称（可选）
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "型号");
    model->setHeaderData(2, Qt::Horizontal, "描述");
    // ... 设置其他表头

    // 隐藏 ID 列（如果不需要显示）
    ui->materialTableView->setColumnHidden(0, true);
    // 初始化状态
    ui->registerButton->setVisible(true);
    ui->loginButton->setVisible(true);
    ui->logoutButton->setVisible(false);
    ui->switchUserButton->setVisible(false);
    ui->currentUserLabel->setVisible(false);

    // 手动连接 QTableView 的 selectionModel 的 selectionChanged 信号到槽函数
    connect(ui->materialTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onMaterialTableViewSelectionChanged);

    connect(ui->viewDrawingButton, &QPushButton::clicked, this, &MainWindow::on_viewDrawingButton_clicked);
    connect(ui->viewPhotoButton, &QPushButton::clicked, this, &MainWindow::on_viewPhotoButton_clicked);
    connect(ui->viewSpecificationButton, &QPushButton::clicked, this, &MainWindow::on_viewSpecificationButton_clicked);


    updateLoginStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    MaterialDialog dialog(currentUser, this);
    //setMaintainer();  // 设置当前用户为物料维护人
    if (dialog.exec() == QDialog::Accepted) {
        // 对话框已经在内部保存了数据，直接刷新模型
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
    MaterialDialog dialog(currentUser, this);
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
    QModelIndex index = model->index(row, 0);  // 假设 id 在第 0 列
    int materialId = model->data(index).toInt();

    // 执行删除操作
    QSqlQuery query;
    query.prepare("DELETE FROM Material WHERE id = :id");
    query.bindValue(":id", materialId);
    if (query.exec()) {
        QMessageBox::information(this, "删除成功", "物料已成功删除。");
        refreshMaterialTable();
    } else {
        QMessageBox::critical(this, "错误", "无法删除物料：" + query.lastError().text());
    }
}


void MainWindow::on_exportButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出Excel", "", "Excel 文件 (*.xlsx)");
    if (!filePath.isEmpty()) {
        QXlsx::Document xlsxDoc;

        // 重新从数据库中查询所有字段的数据
        QSqlQuery query("SELECT * FROM Material");
        int row = 1;
        while (query.next()) {
            for (int col = 0; col < query.record().count(); ++col) {
                QVariant value = query.value(col);
                xlsxDoc.write(row, col + 1, value);
            }
            ++row;
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
    model->setQuery(model->query().lastQuery());  // 重新执行查询
    ui->materialTableView->reset();  // 刷新表格视图
}


void MainWindow::on_viewDrawingButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    QModelIndex index = model->index(row, 0);  // 假设 id 在第 0 列
    int materialId = model->data(index).toInt();

    // 从数据库中查询 drawing 数据
    QSqlQuery query;
    query.prepare("SELECT drawing FROM Material WHERE id = :id");
    query.bindValue(":id", materialId);
    if (query.exec() && query.next()) {
        QByteArray drawingData = query.value(0).toByteArray();
        if (drawingData.isEmpty()) {
            QMessageBox::information(this, "提示", "该物料没有图纸。");
            return;
        }
        // 显示数据
        displayDataInPreview(drawingData);
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
    QModelIndex index = model->index(row, 0);  // 假设 id 在第 0 列
    int materialId = model->data(index).toInt();

    // 从数据库中查询 photo 数据
    QSqlQuery query;
    query.prepare("SELECT photo FROM Material WHERE id = :id");
    query.bindValue(":id", materialId);
    if (query.exec() && query.next()) {
        QByteArray photoData = query.value(0).toByteArray();
        if (photoData.isEmpty()) {
            QMessageBox::information(this, "提示", "该物料没有照片。");
            return;
        }
        // 显示数据
        displayDataInPreview(photoData);
    } else {
        QMessageBox::warning(this, "错误", "无法获取照片数据：" + query.lastError().text());
    }
}

void MainWindow::on_viewSpecificationButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    QModelIndex index = model->index(row, 0);  // 假设 id 在第 0 列
    int materialId = model->data(index).toInt();

    // 从数据库中查询 specification 数据
    QSqlQuery query;
    query.prepare("SELECT specification FROM Material WHERE id = :id");
    query.bindValue(":id", materialId);
    if (query.exec() && query.next()) {
        QByteArray specData = query.value(0).toByteArray();
        if (specData.isEmpty()) {
            QMessageBox::information(this, "提示", "该物料没有供应商规格书。");
            return;
        }
        // 显示数据
        displayDataInPreview(specData);
    } else {
        QMessageBox::warning(this, "错误", "无法获取供应商规格书数据：" + query.lastError().text());
    }
}

void MainWindow::displayDataInPreview(const QByteArray &data)
{
    // 尝试将数据作为图片加载
    QPixmap pixmap;
    if (pixmap.loadFromData(data)) {
        // 如果成功加载为图片，显示在预览区
        ui->previewLabel->setPixmap(pixmap.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // 如果不是图片，尝试将数据作为PDF加载
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QBuffer buffer;
        buffer.setData(data);
        buffer.open(QIODevice::ReadOnly);

        QPdfDocument pdfDoc;
        pdfDoc.load(&buffer);  // 加载 PDF 数据
        if (pdfDoc.status() == QPdfDocument::Status::Ready) {  // 检查加载状态
            // 获取第一页的尺寸
            QSizeF pageSizeF = pdfDoc.pagePointSize(0);
            QSize pageSize = pageSizeF.toSize();  // 转换为 QSize

            // 调用新的 render() 方法，获取渲染后的 QImage
            QImage renderedImage = pdfDoc.render(0, pageSize);

            if (!renderedImage.isNull()) {
                ui->previewLabel->setPixmap(QPixmap::fromImage(renderedImage).scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                QMessageBox::warning(this, "警告", "无法渲染PDF文件。");
            }
        } else {
            QMessageBox::warning(this, "警告", "文件格式不支持或加载失败。");
        }
#else
        QMessageBox::warning(this, "警告", "当前Qt版本不支持PDF渲染。");
#endif
    }
}

// void MainWindow::setMaintainer()
// {
//     // 将物料维护人设置为当前用户
//     if (!currentUser.isEmpty()) {
//         //materialDialog->setCurrentMaintainer(currentUser);  // 假设在MaterialDialog中有相应的函数
//     }
// }
