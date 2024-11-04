// bomdialog.cpp

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
#include <QSqlField> // 添加 QSqlField 头文件

BOMDialog::BOMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BOMDialog),
    materialListModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // 初始化物料清单模型
    materialListModel->setColumnCount(9);  // 修改为9列
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
    ui->bomDescriptionTextEdit->setText(record.value("description").toString()); // 加载描述
    ui->bomRemarksTextEdit->setText(record.value("remarks").toString());         // 加载备注

    // 清空现有数据
    bomMaterials.clear();
    materialListModel->removeRows(0, materialListModel->rowCount());

    // 加载物料清单，使用JOIN获取物料和供应商信息
    QSqlQuery query;
    query.prepare(R"(
        SELECT m.id, m.material_number, m.version, m.description, m.remarks, m.unit_price, s.supplier_name, bm.quantity
        FROM BOM_Material bm
        JOIN Material m ON bm.material_id = m.id
        LEFT JOIN Suppliers s ON m.supplier_id = s.supplier_id
        WHERE bm.bom_id = :bom_id
    )");
    query.bindValue(":bom_id", record.value("id"));
    if (query.exec()) {
        while (query.next()) {
            BOMMaterial bm;
            bm.materialId = query.value("id").toInt();
            bm.materialNumber = query.value("material_number").toString();
            bm.version = query.value("version").toInt();
            bm.description = query.value("description").toString();
            bm.remarks = query.value("remarks").toString();
            bm.unitPrice = query.value("unit_price").toDouble();
            bm.supplier = query.value("supplier_name").toString();
            bm.quantity = query.value("quantity").toInt();
            bm.source = "采购"; // 默认来源为采购，根据需要调整
            bm.bomPrice = bm.unitPrice * bm.quantity;

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
    QSqlRecord record;

    // 定义字段，使用 QMetaType 代替 QVariant::Type
    record.append(QSqlField("id", QMetaType::fromType<int>()));
    record.append(QSqlField("name", QMetaType::fromType<QString>()));
    record.append(QSqlField("description", QMetaType::fromType<QString>())); // 新增描述
    record.append(QSqlField("remarks", QMetaType::fromType<QString>()));     // 新增备注

    // 设置值
    record.setValue("id", bomRecord.value("id")); // 保持ID
    record.setValue("name", ui->bomNameLineEdit->text().trimmed());
    record.setValue("description", ui->bomDescriptionTextEdit->toPlainText().trimmed());
    record.setValue("remarks", ui->bomRemarksTextEdit->toPlainText().trimmed());

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
        bm.materialId = selectedMaterial.value("id").toInt();
        bm.materialNumber = selectedMaterial.value("material_number").toString();
        bm.version = selectedMaterial.value("version").toInt();
        bm.description = selectedMaterial.value("description").toString();
        bm.remarks = selectedMaterial.value("remarks").toString();
        bm.unitPrice = selectedMaterial.value("unit_price").toDouble();
        bm.quantity = quantity;

        QString supplierId = selectedMaterial.value("supplier_id").toString();
        if (!supplierId.isEmpty()) {
            QSqlQuery supplierQuery;
            supplierQuery.prepare("SELECT supplier_name FROM Suppliers WHERE supplier_id = :supplier_id");
            supplierQuery.bindValue(":supplier_id", supplierId);
            if (supplierQuery.exec() && supplierQuery.next()) {
                bm.supplier = supplierQuery.value("supplier_name").toString();
            } else {
                bm.supplier = "未知供应商";
            }
        } else {
            bm.supplier = "未知供应商";
        }

        bm.source = "采购"; // 默认来源为采购，根据需要调整
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
        query.prepare("INSERT INTO BOM (name, description, remarks) VALUES (:name, :description, :remarks)");
        query.bindValue(":name", record.value("name"));
        query.bindValue(":description", record.value("description"));
        query.bindValue(":remarks", record.value("remarks"));
        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(this, "错误", "无法保存 BOM 信息：" + query.lastError().text());
            return;
        }
        int bomId = query.lastInsertId().toInt();
        record.setValue("id", bomId);
    } else {
        // 更新 BOM
        query.prepare("UPDATE BOM SET name = :name, description = :description, remarks = :remarks WHERE id = :id");
        query.bindValue(":id", record.value("id"));
        query.bindValue(":name", record.value("name"));
        query.bindValue(":description", record.value("description"));
        query.bindValue(":remarks", record.value("remarks"));
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
        return;
    }

    // 插入新的物料清单（使用批量插入）
    query.prepare("INSERT INTO BOM_Material (bom_id, material_id, quantity) VALUES (?, ?, ?)");
    for (const BOMMaterial &bm : bomMaterials) {
        query.addBindValue(bomId);
        query.addBindValue(bm.materialId);
        query.addBindValue(bm.quantity);
    }

    if (!query.execBatch()) {
        db.rollback();
        QMessageBox::warning(this, "警告", "无法保存物料清单：" + query.lastError().text());
        return;
    }

    // 提交事务
    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "错误", "无法提交事务：" + db.lastError().text());
        return;
    }

    QMessageBox::information(this, "成功", "BOM已成功保存！");
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
