#include "selectmaterialdialog.h"
#include "ui_selectmaterialdialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItem>
#include <QMessageBox>
#include <QSqlField>

SelectMaterialDialog::SelectMaterialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectMaterialDialog),
    materialModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // 初始化模型
    materialModel->setColumnCount(5);
    QStringList headers;
    headers << "物料号" << "版本号" << "描述" << "规格" << "单价(元)";
    materialModel->setHorizontalHeaderLabels(headers);
    ui->materialTableView->setModel(materialModel);
    ui->materialTableView->horizontalHeader()->setStretchLastSection(true);
    ui->materialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->materialTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    loadMaterials();
}

SelectMaterialDialog::~SelectMaterialDialog()
{
    delete ui;
}

void SelectMaterialDialog::loadMaterials(const QString &filter)
{
    materialModel->removeRows(0, materialModel->rowCount());

    QSqlQuery query;
    if (filter.isEmpty()) {
        query.prepare("SELECT material_number, version, description, remarks, unit_price FROM Material");
    } else {
        query.prepare("SELECT material_number, version, description, remarks, unit_price FROM Material WHERE material_number LIKE :filter OR description LIKE :filter");
        query.bindValue(":filter", "%" + filter + "%");
    }

    if (query.exec()) {
        while (query.next()) {
            QList<QStandardItem*> rowItems;
            rowItems << new QStandardItem(query.value("material_number").toString());
            rowItems << new QStandardItem(QString::number(query.value("version").toInt()));
            rowItems << new QStandardItem(query.value("description").toString());
            rowItems << new QStandardItem(query.value("remarks").toString());
            rowItems << new QStandardItem(QString::number(query.value("unit_price").toDouble(), 'f', 2));
            materialModel->appendRow(rowItems);
        }
    } else {
        QMessageBox::warning(this, "警告", "无法加载物料数据：" + query.lastError().text());
    }
}

void SelectMaterialDialog::on_searchButton_clicked()
{
    QString filter = ui->searchLineEdit->text().trimmed();
    loadMaterials(filter);
}

void SelectMaterialDialog::on_materialTableView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QString materialNumber = materialModel->item(index.row(), 0)->text();
    int version = materialModel->item(index.row(), 1)->text().toInt();
    QString description = materialModel->item(index.row(), 2)->text();
    QString remarks = materialModel->item(index.row(), 3)->text();
    double unitPrice = materialModel->item(index.row(), 4)->text().toDouble();

    QSqlRecord record;
    record.append(QSqlField("material_number", QVariant::String));
    record.append(QSqlField("version", QVariant::Int));
    record.append(QSqlField("description", QVariant::String));
    record.append(QSqlField("remarks", QVariant::String));
    record.append(QSqlField("unit_price", QVariant::Double));

    record.setValue("material_number", materialNumber);
    record.setValue("version", version);
    record.setValue("description", description);
    record.setValue("remarks", remarks);
    record.setValue("unit_price", unitPrice);

    selectedMaterial = record;
    accept();
}

QSqlRecord SelectMaterialDialog::getSelectedMaterial() const
{
    return selectedMaterial;
}
