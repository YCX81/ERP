#include "bomdialog.h"
#include "ui_bomdialog.h"
#include "selectmaterialdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QStandardItem>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlDatabase>

BOMDialog::BOMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BOMDialog),
    materialListModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // 初始化物料清单模型
    materialListModel->setColumnCount(10);
    QStringList headers;
    headers << "物料号" << "版本号" << "描述" << "备注"
            << "数量" << "来源" << "供应商" << "单价(元)" << "BOM单价(元)";
    materialListModel->setHorizontalHeaderLabels(headers);
    ui->materialTableView->setModel(materialListModel);
    ui->materialTableView->horizontalHeader()->setStretchLastSection(true);
    ui->materialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->materialTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 初始化总成本
    ui->totalCostValueLabel->setText("0.00");
}

BOMDialog::~BOMDialog()
{
    delete ui;
}

void BOMDialog::setBOMData(const QSqlRecord &record)
{
    bomRecord = record;

    ui->bomNameLineEdit->setText(record.value("name").toString());

    // 清空现有数据
    bomMaterials.clear();
    materialListModel->removeRows(0, materialListModel->rowCount());

    // 加载物料清单
    QSqlQuery query;
    query.prepare("SELECT material_id, quantity FROM BOM_Material WHERE bom_id = :bom_id");
    query.bindValue(":bom_id", record.value("id"));
    if (query.exec()) {
        while (query.next()) {
            BOMMaterial bm;
            bm.materialId = query.value("material_id").toInt();
            bm.quantity = query.value("quantity").toInt();

            // 查询物料详细信息
            QSqlQuery materialQuery;
            materialQuery.prepare("SELECT material_number, version, description, remarks, unit_price, supplier_id FROM Material WHERE id = :id");
            materialQuery.bindValue(":id", bm.materialId);
            if (materialQuery.exec() && materialQuery.next()) {
                bm.materialNumber = materialQuery.value("material_number").toString();
                bm.version = materialQuery.value("version").toInt();
                bm.description = materialQuery.value("description").toString();
                bm.remarks = materialQuery.value("remarks").toString();
                bm.unitPrice = materialQuery.value("unit_price").toDouble();

                // 查询供应商名称
                QString supplierId = materialQuery.value("supplier_id").toString();
                QSqlQuery supplierQuery;
                supplierQuery.prepare("SELECT supplier_name FROM Suppliers WHERE supplier_id = :supplier_id");
                supplierQuery.bindValue(":supplier_id", supplierId);
                if (supplierQuery.exec() && supplierQuery.next()) {
                    bm.supplier = supplierQuery.value("supplier_name").toString();
                } else {
                    bm.supplier = "未知供应商";
                }

                bm.source = "采购"; // 默认来源为采购，根据需要调整

                //bm.usageMM = ""; // 根据实际情况填写，如果有数据来源

                bm.bomPrice = bm.unitPrice * bm.quantity;
            } else {
                bm.materialNumber = "未知物料";
                bm.version = 0;
                bm.description = "未知物料";
                bm.remarks = "";
                bm.unitPrice = 0.0;
                bm.supplier = "未知供应商";
                bm.source = "未知";
                //bm.usageMM = "";
                bm.bomPrice = 0.0;
            }

            bomMaterials.append(bm);
        }
        updateMaterialTable();
        calculateTotalCost();
    } else {
        QMessageBox::warning(this, "警告", "无法加载物料清单：" + query.lastError().text());
    }
}

QSqlRecord BOMDialog::getBOMData() const
{
    QSqlRecord record = bomRecord;
    record.setValue("name", ui->bomNameLineEdit->text().trimmed());
    // 其他字段根据需要添加
    return record;
}

void BOMDialog::on_addMaterialButton_clicked()
{
    SelectMaterialDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        QSqlRecord selectedMaterial = dlg.getSelectedMaterial();
        if (selectedMaterial.isEmpty()) {
            QMessageBox::warning(this, "警告", "未选择有效的物料。");
            return;
        }

        // 输入数量
        bool ok;
        int quantity = QInputDialog::getInt(this, "输入数量", "数量：", 1, 1, 1000000, 1, &ok);
        if (!ok) return;

        BOMMaterial bm;
        bm.materialNumber = selectedMaterial.value("material_number").toString();
        bm.version = selectedMaterial.value("version").toInt();
        bm.description = selectedMaterial.value("description").toString();
        bm.remarks = selectedMaterial.value("remarks").toString();
        bm.unitPrice = selectedMaterial.value("unit_price").toDouble();
        bm.quantity = quantity;

        // 查询物料ID和供应商名称
        QSqlQuery materialQuery;
        materialQuery.prepare("SELECT id, supplier_id FROM Material WHERE material_number = :material_number");
        materialQuery.bindValue(":material_number", bm.materialNumber);
        if (materialQuery.exec() && materialQuery.next()) {
            bm.materialId = materialQuery.value("id").toInt();
            QString supplierId = materialQuery.value("supplier_id").toString();

            QSqlQuery supplierQuery;
            supplierQuery.prepare("SELECT supplier_name FROM Suppliers WHERE supplier_id = :supplier_id");
            supplierQuery.bindValue(":supplier_id", supplierId);
            if (supplierQuery.exec() && supplierQuery.next()) {
                bm.supplier = supplierQuery.value("supplier_name").toString();
            } else {
                bm.supplier = "未知供应商";
            }
        } else {
            QMessageBox::warning(this, "警告", "无法获取物料的详细信息。");
            return;
        }

        bm.source = "采购"; // 默认来源为采购，根据需要调整
        //bm.usageMM = ""; // 根据实际情况填写，如果有数据来源
        bm.bomPrice = bm.unitPrice * bm.quantity;

        bomMaterials.append(bm);
        updateMaterialTable();
        calculateTotalCost();
    }
}

void BOMDialog::on_removeMaterialButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        int row = selectedRows.first().row();
        bomMaterials.removeAt(row);
        updateMaterialTable();
        calculateTotalCost();
    } else {
        QMessageBox::warning(this, "警告", "请先选择要删除的物料。");
    }
}

void BOMDialog::on_saveButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    QSqlRecord record = getBOMData();

    QSqlQuery query;
    bool isNew = record.value("id").isNull() || record.value("id").toInt() == 0;

    // 开始事务
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::critical(this, "错误", "无法启动事务：" + db.lastError().text());
        return;
    }

    if (isNew) {
        // 新增 BOM
        query.prepare("INSERT INTO BOM (name, description) VALUES (:name, :description)");
        query.bindValue(":name", record.value("name"));
        query.bindValue(":description", ""); // 根据需要填写
        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(this, "错误", "无法保存 BOM 信息：" + query.lastError().text());
            return;
        }
        int bomId = query.lastInsertId().toInt();
        record.setValue("id", bomId);
    } else {
        // 更新 BOM
        query.prepare("UPDATE BOM SET name = :name, description = :description WHERE id = :id");
        query.bindValue(":id", record.value("id"));
        query.bindValue(":name", record.value("name"));
        query.bindValue(":description", ""); // 根据需要填写
        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(this, "错误", "无法更新 BOM 信息：" + query.lastError().text());
            return;
        }
    }

    int bomId = record.value("id").toInt();

    // 清除旧的物料清单
    query.prepare("DELETE FROM BOM_Material WHERE bom_id = :bom_id");
    query.bindValue(":bom_id", bomId);
    if (!query.exec()) {
        db.rollback();
        QMessageBox::warning(this, "警告", "无法清除旧的物料清单：" + query.lastError().text());
    }

    // 插入新的物料清单
    for (const BOMMaterial &bm : bomMaterials) {
        query.prepare("INSERT INTO BOM_Material (bom_id, material_id, quantity) VALUES (:bom_id, :material_id, :quantity)");
        query.bindValue(":bom_id", bomId);
        query.bindValue(":material_id", bm.materialId);
        query.bindValue(":quantity", bm.quantity);
        if (!query.exec()) {
            db.rollback();
            QMessageBox::warning(this, "警告", "无法保存物料清单：" + query.lastError().text());
            return;
        }
    }

    // 提交事务
    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "错误", "无法提交事务：" + db.lastError().text());
        return;
    }

    accept();
}

void BOMDialog::on_cancelButton_clicked()
{
    reject();
}

bool BOMDialog::validateInput()
{
    if (ui->bomNameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "BOM 名称不能为空。");
        return false;
    }
    if (bomMaterials.isEmpty()) {
        QMessageBox::warning(this, "警告", "物料清单不能为空。");
        return false;
    }
    return true;
}

void BOMDialog::updateMaterialTable()
{
    materialListModel->removeRows(0, materialListModel->rowCount());

    for (const BOMMaterial &bm : bomMaterials) {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(bm.materialNumber);
        rowItems << new QStandardItem(QString::number(bm.version));
        rowItems << new QStandardItem(bm.description);
        rowItems << new QStandardItem(bm.remarks);
        //rowItems << new QStandardItem(bm.usageMM.isEmpty() ? "-" : bm.usageMM);
        rowItems << new QStandardItem(QString::number(bm.quantity));
        rowItems << new QStandardItem(bm.source);
        rowItems << new QStandardItem(bm.supplier);
        rowItems << new QStandardItem(QString::number(bm.unitPrice, 'f', 2));
        rowItems << new QStandardItem(QString::number(bm.bomPrice, 'f', 2));
        materialListModel->appendRow(rowItems);
    }
}

void BOMDialog::calculateTotalCost()
{
    double totalCost = 0.0;
    for (const BOMMaterial &bm : bomMaterials) {
        totalCost += bm.bomPrice;
    }
    ui->totalCostValueLabel->setText(QString::number(totalCost, 'f', 2));
}
