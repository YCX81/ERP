#include "supplierdialog.h"
#include "ui_supplierdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

SupplierDialog::SupplierDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupplierDialog)
{
    ui->setupUi(this);
}

SupplierDialog::~SupplierDialog()
{
    delete ui;
}

void SupplierDialog::setSupplierData(const QSqlRecord &record)
{
    supplierRecord = record;

    ui->supplierIdLineEdit->setText(record.value("supplier_id").toString());
    ui->supplierNameLineEdit->setText(record.value("supplier_name").toString());
}

QSqlRecord SupplierDialog::getSupplierData() const
{
    QSqlRecord record = supplierRecord;

    record.setValue("supplier_id", ui->supplierIdLineEdit->text().trimmed());
    record.setValue("supplier_name", ui->supplierNameLineEdit->text().trimmed());

    return record;
}

void SupplierDialog::on_saveButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    QSqlRecord record = getSupplierData();

    QSqlQuery query;
    if (record.value("id").isNull() || record.value("id").toInt() == 0) {
        // 新增供应商
        query.prepare("INSERT INTO Suppliers (supplier_id, supplier_name) "
                      "VALUES (:supplier_id, :supplier_name)");
    } else {
        // 更新供应商
        query.prepare("UPDATE Suppliers SET supplier_id = :supplier_id, supplier_name = :supplier_name "
                      "WHERE id = :id");
        query.bindValue(":id", record.value("id"));
    }
    query.bindValue(":supplier_id", record.value("supplier_id"));
    query.bindValue(":supplier_name", record.value("supplier_name"));

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "无法保存供应商信息：" + query.lastError().text());
        return;
    }

    accept();
}

void SupplierDialog::on_cancelButton_clicked()
{
    reject();
}

bool SupplierDialog::validateInput()
{
    QString supplierId = ui->supplierIdLineEdit->text().trimmed();
    QString supplierName = ui->supplierNameLineEdit->text().trimmed();

    if (supplierId.isEmpty()) {
        QMessageBox::warning(this, "警告", "供应商编号不能为空。");
        return false;
    }
    if (supplierId.length() != 3) {
        QMessageBox::warning(this, "警告", "供应商编号必须是3位字符。");
        return false;
    }
    if (supplierName.isEmpty()) {
        QMessageBox::warning(this, "警告", "供应商名称不能为空。");
        return false;
    }
    return true;
}
