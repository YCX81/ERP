#include "supplierdialog.h"
#include "ui_supplierdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlField>

SupplierDialog::SupplierDialog(const QString &currentUser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupplierDialog),
    currentUser(currentUser)
{
    ui->setupUi(this);

    // 初始化经营性质下拉框
    ui->businessNatureComboBox->addItem("实体");
    ui->businessNatureComboBox->addItem("淘宝");
    ui->businessNatureComboBox->addItem("电商");
    // 根据需要添加更多选项

    // 初始化供应商负责人下拉框
    ui->supplierManagerComboBox->addItem(currentUser); // 默认选中当前用户
    QSqlQuery query("SELECT DISTINCT supplier_manager FROM Suppliers WHERE supplier_manager IS NOT NULL AND supplier_manager != ''");
    while (query.next()) {
        QString manager = query.value(0).toString();
        if (!manager.isEmpty() && manager != currentUser) {
            ui->supplierManagerComboBox->addItem(manager);
        }
    }

    // 设置默认供应商负责人为当前用户
    int index = ui->supplierManagerComboBox->findText(currentUser);
    if (index != -1) {
        ui->supplierManagerComboBox->setCurrentIndex(index);
    }
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

    // 设置经营性质
    QString businessNature = record.value("business_nature").toString();
    int businessIndex = ui->businessNatureComboBox->findText(businessNature);
    if (businessIndex != -1) {
        ui->businessNatureComboBox->setCurrentIndex(businessIndex);
    } else {
        ui->businessNatureComboBox->setCurrentIndex(-1); // 未选择
    }

    // 设置第一联系人及电话
    ui->primaryContactLineEdit->setText(record.value("primary_contact").toString());
    ui->primaryContactPhoneLineEdit->setText(record.value("primary_contact_phone").toString());

    // 设置第二联系人及电话
    ui->secondaryContactLineEdit->setText(record.value("secondary_contact").toString());
    ui->secondaryContactPhoneLineEdit->setText(record.value("secondary_contact_phone").toString());

    // 设置地址
    ui->addressLineEdit->setText(record.value("address").toString());

    // 设置供应商负责人
    QString supplierManager = record.value("supplier_manager").toString();
    int managerIndex = ui->supplierManagerComboBox->findText(supplierManager);
    if (managerIndex != -1) {
        ui->supplierManagerComboBox->setCurrentIndex(managerIndex);
    } else {
        ui->supplierManagerComboBox->setCurrentIndex(-1); // 未选择
    }
}

QSqlRecord SupplierDialog::getSupplierData() const
{
    QSqlRecord record;

    // 假设数据库表有一个自增主键字段 "id"
    // 需要从原始记录中获取 "id" 字段
    if (supplierRecord.contains("id")) {
        record.append(QSqlField("id", QVariant::Int));
        record.setValue("id", supplierRecord.value("id").toInt());
    }

    record.append(QSqlField("supplier_id", QVariant::String));
    record.append(QSqlField("supplier_name", QVariant::String));
    record.append(QSqlField("business_nature", QVariant::String));
    record.append(QSqlField("primary_contact", QVariant::String));
    record.append(QSqlField("primary_contact_phone", QVariant::String));
    record.append(QSqlField("secondary_contact", QVariant::String));
    record.append(QSqlField("secondary_contact_phone", QVariant::String));
    record.append(QSqlField("address", QVariant::String));
    record.append(QSqlField("supplier_manager", QVariant::String));

    // 设置值
    record.setValue("supplier_id", ui->supplierIdLineEdit->text().trimmed());
    record.setValue("supplier_name", ui->supplierNameLineEdit->text().trimmed());
    record.setValue("business_nature", ui->businessNatureComboBox->currentText());
    record.setValue("primary_contact", ui->primaryContactLineEdit->text().trimmed());
    record.setValue("primary_contact_phone", ui->primaryContactPhoneLineEdit->text().trimmed());
    record.setValue("secondary_contact", ui->secondaryContactLineEdit->text().trimmed());
    record.setValue("secondary_contact_phone", ui->secondaryContactPhoneLineEdit->text().trimmed());
    record.setValue("address", ui->addressLineEdit->text().trimmed());
    record.setValue("supplier_manager", ui->supplierManagerComboBox->currentText());

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
        query.prepare("INSERT INTO Suppliers (supplier_id, supplier_name, business_nature, primary_contact, "
                      "primary_contact_phone, secondary_contact, secondary_contact_phone, address, supplier_manager) "
                      "VALUES (:supplier_id, :supplier_name, :business_nature, :primary_contact, "
                      ":primary_contact_phone, :secondary_contact, :secondary_contact_phone, :address, :supplier_manager)");
    } else {
        // 更新供应商
        query.prepare("UPDATE Suppliers SET supplier_id = :supplier_id, supplier_name = :supplier_name, "
                      "business_nature = :business_nature, primary_contact = :primary_contact, "
                      "primary_contact_phone = :primary_contact_phone, secondary_contact = :secondary_contact, "
                      "secondary_contact_phone = :secondary_contact_phone, address = :address, "
                      "supplier_manager = :supplier_manager WHERE id = :id");
        query.bindValue(":id", record.value("id"));
    }

    // 绑定数据到查询语句
    query.bindValue(":supplier_id", record.value("supplier_id"));
    query.bindValue(":supplier_name", record.value("supplier_name"));
    query.bindValue(":business_nature", record.value("business_nature"));
    query.bindValue(":primary_contact", record.value("primary_contact"));
    query.bindValue(":primary_contact_phone", record.value("primary_contact_phone"));
    query.bindValue(":secondary_contact", record.value("secondary_contact"));
    query.bindValue(":secondary_contact_phone", record.value("secondary_contact_phone"));
    query.bindValue(":address", record.value("address"));
    query.bindValue(":supplier_manager", record.value("supplier_manager"));

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "无法保存供应商信息：" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "成功", "供应商信息已成功保存。");
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
    QString businessNature = ui->businessNatureComboBox->currentText().trimmed();
    QString supplierManager = ui->supplierManagerComboBox->currentText().trimmed();

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
    if (businessNature.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择经营性质。");
        return false;
    }
    if (supplierManager.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择供应商负责人。");
        return false;
    }
    // 可以添加更多验证规则，例如电话号码格式、地址长度等

    return true;
}
