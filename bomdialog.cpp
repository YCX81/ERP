#include "bomdialog.h"
#include "ui_bomdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>

BOMDialog::BOMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BOMDialog)
{
    ui->setupUi(this);

    // 初始化物料清单模型
    materialListModel = new QStandardItemModel(this);
    materialListModel->setHorizontalHeaderLabels(QStringList() << "物料ID" << "物料名称" << "数量");
    ui->materialTableView->setModel(materialListModel);
}

BOMDialog::~BOMDialog()
{
    delete ui;
}

void BOMDialog::setBOMData(const QSqlRecord &record)
{
    bomRecord = record;

    ui->bomNameLineEdit->setText(record.value("name").toString());

    // 加载物料清单
    QSqlQuery query;
    query.prepare("SELECT material_id, quantity FROM BOM_Material WHERE bom_id = :bom_id");
    query.bindValue(":bom_id", record.value("id"));
    if (query.exec()) {
        while (query.next()) {
            QList<QStandardItem*> rowItems;
            int materialId = query.value("material_id").toInt();
            rowItems << new QStandardItem(QString::number(materialId));

            // 查询物料名称
            QSqlQuery materialQuery;
            materialQuery.prepare("SELECT model FROM Material WHERE id = :id");
            materialQuery.bindValue(":id", materialId);
            if (materialQuery.exec() && materialQuery.next()) {
                rowItems << new QStandardItem(materialQuery.value("model").toString());
            } else {
                rowItems << new QStandardItem("未知物料");
            }

            rowItems << new QStandardItem(query.value("quantity").toString());
            materialListModel->appendRow(rowItems);
        }
    } else {
        QMessageBox::warning(this, "警告", "无法加载物料清单：" + query.lastError().text());
    }
}

QSqlRecord BOMDialog::getBOMData() const
{
    QSqlRecord record = bomRecord;
    record.setValue("name", ui->bomNameLineEdit->text());
    return record;
}

void BOMDialog::on_addMaterialButton_clicked()
{
    // 简化为手动输入物料ID和数量
    bool ok;
    int materialId = QInputDialog::getInt(this, "添加物料", "请输入物料ID：", 0, 0, 2147483647, 1, &ok);
    if (ok) {
        int quantity = QInputDialog::getInt(this, "添加物料", "请输入数量：", 1, 1, 2147483647, 1, &ok);
        if (ok) {
            QList<QStandardItem*> rowItems;
            rowItems << new QStandardItem(QString::number(materialId));

            // 查询物料名称
            QSqlQuery materialQuery;
            materialQuery.prepare("SELECT model FROM Material WHERE id = :id");
            materialQuery.bindValue(":id", materialId);
            if (materialQuery.exec() && materialQuery.next()) {
                rowItems << new QStandardItem(materialQuery.value("model").toString());
            } else {
                rowItems << new QStandardItem("未知物料");
            }

            rowItems << new QStandardItem(QString::number(quantity));
            materialListModel->appendRow(rowItems);
        }
    }
}

void BOMDialog::on_removeMaterialButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        materialListModel->removeRow(selectedRows.first().row());
    }
}

void BOMDialog::on_saveButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    QSqlRecord record = getBOMData();

    QSqlQuery query;
    if (record.value("id").isNull() || record.value("id").toInt() == 0) {
        // 新增 BOM
        query.prepare("INSERT INTO BOM (name) VALUES (:name)");
        query.bindValue(":name", record.value("name"));
        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "无法保存 BOM 信息：" + query.lastError().text());
            return;
        }
        int bomId = query.lastInsertId().toInt();
        record.setValue("id", bomId);
        bomRecord = record;
    } else {
        // 更新 BOM
        query.prepare("UPDATE BOM SET name = :name WHERE id = :id");
        query.bindValue(":id", record.value("id"));
        query.bindValue(":name", record.value("name"));
        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "无法更新 BOM 信息：" + query.lastError().text());
            return;
        }
    }

    int bomId = record.value("id").toInt();

    // 保存物料清单
    query.prepare("DELETE FROM BOM_Material WHERE bom_id = :bom_id");
    query.bindValue(":bom_id", bomId);
    if (!query.exec()) {
        QMessageBox::warning(this, "警告", "无法清除旧的物料清单：" + query.lastError().text());
    }

    for (int row = 0; row < materialListModel->rowCount(); ++row) {
        int materialId = materialListModel->item(row, 0)->text().toInt();
        int quantity = materialListModel->item(row, 2)->text().toInt();

        query.prepare("INSERT INTO BOM_Material (bom_id, material_id, quantity) VALUES (:bom_id, :material_id, :quantity)");
        query.bindValue(":bom_id", bomId);
        query.bindValue(":material_id", materialId);
        query.bindValue(":quantity", quantity);
        if (!query.exec()) {
            QMessageBox::warning(this, "警告", "无法保存物料清单：" + query.lastError().text());
        }
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
    if (materialListModel->rowCount() == 0) {
        QMessageBox::warning(this, "警告", "物料清单不能为空。");
        return false;
    }
    return true;
}
