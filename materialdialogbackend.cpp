#include "materialdialogbackend.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>

MaterialDialogBackend::MaterialDialogBackend(const QString &currentUser, QObject *parent)
    : QObject(parent),
    m_currentMaintainer(currentUser)
{
    qRegisterMetaType<CategoryInfo>("CategoryInfo");
    initializeCategories();
    initializeSuppliers();
}

QList<QVariant> MaterialDialogBackend::getCategoryList() const
{
    QList<QVariant> list;
    for (const CategoryInfo &category : m_categoryList) {
        list.append(QVariant::fromValue(category));
    }
    return list;
}

QList<QVariant> MaterialDialogBackend::getSupplierList() const
{
    return m_supplierList;
}

QString MaterialDialogBackend::currentMaintainer() const
{
    return m_currentMaintainer;
}

void MaterialDialogBackend::setCurrentMaintainer(const QString &maintainer)
{
    if (m_currentMaintainer != maintainer) {
        m_currentMaintainer = maintainer;
        emit currentMaintainerChanged();
    }
}

void MaterialDialogBackend::initializeCategories()
{
    m_categoryList = {
        {10, "线束连接器", "XL"},
        {16, "紧固件", "TS"},
        {17, "胶水", "GL"},
        {18, "标签及辅材", "LA"},
        {4,  "开关", "SW"},
        {3,  "滚轮", "RO"},
        {5,  "摇杆", "MJ"},
        {1,  "电控手柄", "JC"},
        {6,  "电子油门", "MO"},
        {7,  "传感器", "SE"},
        {8,  "电路板", "PC"},
        {9,  "电子件", "EM"},
        {2,  "半成品", "SM"},
        {11, "模具五金", "TM"},
        {12, "机加工五金", "CM"},
        {13, "模具塑料", "TP"},
        {14, "机加工塑料", "CP"},
        {15, "橡胶", "RB"}
    };
    emit categoryListChanged();
}

void MaterialDialogBackend::initializeSuppliers()
{
    QSqlQuery supplierQuery("SELECT supplier_id, supplier_name FROM Suppliers");
    while (supplierQuery.next()) {
        QString supplierID = supplierQuery.value("supplier_id").toString();
        QString supplierName = supplierQuery.value("supplier_name").toString();
        QString displayText = QString("%1 - %2").arg(supplierID, supplierName);
        m_supplierList.append(QVariant::fromValue(displayText));
    }
    emit supplierListChanged();
}

QString MaterialDialogBackend::generateMaterialNumber(const QString &categoryLetterCode, const QString &supplierID, int versionNumber)
{
    QSqlQuery query;
    query.prepare("SELECT MAX(CAST(SUBSTRING(material_number, 5, 3) AS UNSIGNED)) FROM Material WHERE material_number LIKE :pattern");
    QString pattern = QString("%1%2___%").arg(categoryLetterCode).arg(supplierID);
    query.bindValue(":pattern", pattern);
    int maxCode = 0;
    if (query.exec() && query.next()) {
        maxCode = query.value(0).toInt();
    }
    int newCode = maxCode + 1;
    QString autoGeneratedCode = QString::number(newCode).rightJustified(3, '0');

    QString materialNumber = QString("%1%2%3").arg(categoryLetterCode).arg(supplierID).arg(autoGeneratedCode);

    if (versionNumber != 0) {
        materialNumber += QString("MK%1").arg(versionNumber);
    }

    return materialNumber;
}

bool MaterialDialogBackend::uploadFile(const QString &filePath, QByteArray &data)
{
    if (filePath.isEmpty()) return false;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        data = file.readAll();
        file.close();
        return true;
    }
    return false;
}

bool MaterialDialogBackend::downloadFile(const QString &filePath, const QByteArray &data)
{
    if (filePath.isEmpty()) return false;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        return true;
    }
    return false;
}

QSqlRecord MaterialDialogBackend::getMaterialRecord(const QVariantMap &recordMap)
{
    QSqlRecord record;
    // 假设你的数据库表结构与recordMap中的键一致
    // 设置字段
    record.append(QSqlField("material_number", QVariant::String));
    record.append(QSqlField("description", QVariant::String));
    record.append(QSqlField("quantity", QVariant::Int));
    record.append(QSqlField("unit_price", QVariant::Double));
    record.append(QSqlField("self_made_or_purchase", QVariant::String));
    record.append(QSqlField("version_number", QVariant::Int));
    record.append(QSqlField("serial_number", QVariant::String));
    record.append(QSqlField("location_number", QVariant::String));
    record.append(QSqlField("good_quantity", QVariant::Int));
    record.append(QSqlField("defective_quantity", QVariant::Int));
    record.append(QSqlField("supplier", QVariant::String));
    record.append(QSqlField("supplier_material_number", QVariant::String));
    record.append(QSqlField("delivery_period", QVariant::Int));
    record.append(QSqlField("material_maintainer", QVariant::String));
    record.append(QSqlField("remarks", QVariant::String));
    record.append(QSqlField("update_date", QVariant::DateTime));
    record.append(QSqlField("drawing", QVariant::ByteArray));
    record.append(QSqlField("photo", QVariant::ByteArray));

    // 绑定值
    record.setValue("material_number", recordMap.value("material_number").toString());
    record.setValue("description", recordMap.value("description").toString());
    record.setValue("quantity", recordMap.value("quantity").toInt());
    record.setValue("unit_price", recordMap.value("unit_price").toDouble());
    record.setValue("self_made_or_purchase", recordMap.value("self_made_or_purchase").toString());
    record.setValue("version_number", recordMap.value("version_number").toInt());
    record.setValue("serial_number", recordMap.value("serial_number").toString());
    record.setValue("location_number", recordMap.value("location_number").toString());
    record.setValue("good_quantity", recordMap.value("good_quantity").toInt());
    record.setValue("defective_quantity", recordMap.value("defective_quantity").toInt());
    record.setValue("supplier", recordMap.value("supplier").toString());
    record.setValue("supplier_material_number", recordMap.value("supplier_material_number").toString());
    record.setValue("delivery_period", recordMap.value("delivery_period").toInt());
    record.setValue("material_maintainer", recordMap.value("material_maintainer").toString());
    record.setValue("remarks", recordMap.value("remarks").toString());
    record.setValue("update_date", QDateTime::fromString(recordMap.value("update_date").toString(), Qt::ISODate));
    record.setValue("drawing", drawingData);
    record.setValue("photo", photoData);

    return record;
}

bool MaterialDialogBackend::saveMaterialData(const QVariantMap &recordMap)
{
    QString errorMessage;
    if (!validateInput(recordMap, errorMessage)) {
        qDebug() << "Validation failed:" << errorMessage;
        return false;
    }

    QSqlRecord record = getMaterialRecord(recordMap);

    QSqlQuery query;
    if (recordMap.contains("id")) {
        // 更新物料
        query.prepare("UPDATE Material SET material_number = :material_number, description = :description, quantity = :quantity, "
                      "unit_price = :unit_price, self_made_or_purchase = :self_made_or_purchase, version_number = :version_number, "
                      "serial_number = :serial_number, location_number = :location_number, good_quantity = :good_quantity, "
                      "defective_quantity = :defective_quantity, supplier = :supplier, supplier_material_number = :supplier_material_number, "
                      "delivery_period = :delivery_period, material_maintainer = :material_maintainer, update_date = :update_date, "
                      "remarks = :remarks, drawing = :drawing, photo = :photo WHERE id = :id");
        query.bindValue(":id", recordMap.value("id").toInt());
    } else {
        // 新增物料
        query.prepare("INSERT INTO Material (material_number, description, quantity, unit_price, self_made_or_purchase, version_number, "
                      "serial_number, location_number, good_quantity, defective_quantity, supplier, supplier_material_number, "
                      "delivery_period, material_maintainer, update_date, remarks, drawing, photo) "
                      "VALUES (:material_number, :description, :quantity, :unit_price, :self_made_or_purchase, :version_number, "
                      ":serial_number, :location_number, :good_quantity, :defective_quantity, :supplier, :supplier_material_number, "
                      ":delivery_period, :material_maintainer, :update_date, :remarks, :drawing, :photo)");
    }

    // 绑定值
    for (int i = 0; i < record.count(); ++i) {
        query.bindValue(":" + record.fieldName(i), record.value(i));
    }

    if (!query.exec()) {
        qDebug() << "Save failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MaterialDialogBackend::validateInput(const QVariantMap &recordMap, QString &errorMessage)
{
    if (recordMap.value("material_number").toString().trimmed().isEmpty()) {
        errorMessage = "物料号不能为空。";
        return false;
    }
    if (recordMap.value("description").toString().trimmed().isEmpty()) {
        errorMessage = "描述不能为空。";
        return false;
    }
    // 添加其他验证规则
    return true;
}