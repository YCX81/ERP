#include "material.h"
#include "database.h"
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QObject>  // 确保connect函数声明正确
#include <QSqlError> // 确保QSqlError类型完整

QStandardItemModel *materialModel = nullptr; // 实际定义和初始化


void addMaterial()
{
    QDialog addMaterialDialog;
    addMaterialDialog.setWindowTitle("添加物料");

    QFormLayout formLayout(&addMaterialDialog);

    QLineEdit *categoryEdit = new QLineEdit(&addMaterialDialog);
    QLineEdit *descriptionEdit = new QLineEdit(&addMaterialDialog);
    QLineEdit *supplierEdit = new QLineEdit(&addMaterialDialog);
    QDoubleSpinBox *priceEdit = new QDoubleSpinBox(&addMaterialDialog);
    priceEdit->setPrefix("¥ ");
    QSpinBox *quantityEdit = new QSpinBox(&addMaterialDialog);

    formLayout.addRow("类别:", categoryEdit);
    formLayout.addRow("描述:", descriptionEdit);
    formLayout.addRow("供应商:", supplierEdit);
    formLayout.addRow("单价:", priceEdit);
    formLayout.addRow("数量:", quantityEdit);

    QPushButton *addButton = new QPushButton("添加", &addMaterialDialog);
    QObject::connect(addButton, &QPushButton::clicked, &addMaterialDialog, &QDialog::accept);
    formLayout.addRow(addButton);

    if (addMaterialDialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO materials (category, description, supplier, price, quantity) VALUES (:category, :description, :supplier, :price, :quantity)");
        query.bindValue(":category", categoryEdit->text());
        query.bindValue(":description", descriptionEdit->text());
        query.bindValue(":supplier", supplierEdit->text());
        query.bindValue(":price", priceEdit->value());
        query.bindValue(":quantity", quantityEdit->value());

        if (query.exec()) {
            materialModel->select();
            QMessageBox::information(nullptr, "成功", "物料已成功添加！");
        } else {
            QMessageBox::warning(nullptr, "错误", "添加物料失败：" + query.lastError().text());
        }
    }
}

void removeMaterial()
{
    QModelIndex index = materialModel->index(0, 0); // 假设选择了第一个物料
    if (index.isValid()) {
        materialModel->removeRow(index.row());
        materialModel->submitAll();
        QMessageBox::information(nullptr, "删除成功", "物料已删除。");
    }
}

void editMaterial()
{
    QModelIndex index = materialModel->index(0, 0); // 假设选择了第一个物料
    if (index.isValid()) {
        QSqlRecord record = materialModel->record(index.row());

        QDialog editMaterialDialog;
        editMaterialDialog.setWindowTitle("编辑物料");

        QFormLayout formLayout(&editMaterialDialog);

        QLineEdit *categoryEdit = new QLineEdit(record.value("category").toString(), &editMaterialDialog);
        QLineEdit *descriptionEdit = new QLineEdit(record.value("description").toString(), &editMaterialDialog);
        QLineEdit *supplierEdit = new QLineEdit(record.value("supplier").toString(), &editMaterialDialog);
        QDoubleSpinBox *priceEdit = new QDoubleSpinBox(&editMaterialDialog);
        priceEdit->setValue(record.value("price").toDouble());
        QSpinBox *quantityEdit = new QSpinBox(&editMaterialDialog);
        quantityEdit->setValue(record.value("quantity").toInt());

        formLayout.addRow("类别:", categoryEdit);
        formLayout.addRow("描述:", descriptionEdit);
        formLayout.addRow("供应商:", supplierEdit);
        formLayout.addRow("单价:", priceEdit);
        formLayout.addRow("数量:", quantityEdit);

        QPushButton *saveButton = new QPushButton("保存", &editMaterialDialog);
        QObject::connect(saveButton, &QPushButton::clicked, &editMaterialDialog, &QDialog::accept);
        formLayout.addRow(saveButton);

        if (editMaterialDialog.exec() == QDialog::Accepted) {
            record.setValue("category", categoryEdit->text());
            record.setValue("description", descriptionEdit->text());
            record.setValue("supplier", supplierEdit->text());
            record.setValue("price", priceEdit->value());
            record.setValue("quantity", quantityEdit->value());

            materialModel->setRecord(index.row(), record);
            materialModel->submitAll();
            QMessageBox::information(nullptr, "编辑成功", "物料信息已更新。");
        }
    }
}
