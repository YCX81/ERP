#include "materialdialog.h"
#include "ui_materialdialog.h"
#include <QFileDialog>
#include <QBuffer>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QVariant>
#include <QDebug>

MaterialDialog::MaterialDialog(const QString &currentUser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialDialog),
    currentMaintainer(currentUser)
{
    ui->setupUi(this);

    // 设置 sourceComboBox 的选项和用户数据
    ui->sourceComboBox->clear();
    ui->sourceComboBox->addItem("自制 (P)", QVariant("P"));
    ui->sourceComboBox->addItem("采购 (M)", QVariant("M"));

    // 设置 statusComboBox 的选项和用户数据
    ui->statusComboBox->clear();
    ui->statusComboBox->addItem("预生产 (PP)", QVariant("PP"));
    ui->statusComboBox->addItem("正式生产 (FP)", QVariant("FP"));

    // 设置物料维护人为当前用户名
    ui->maintainerLineEdit->setText(currentMaintainer);
    ui->maintainerLineEdit->setReadOnly(true);  // 设置为只读

    // 设置初始更新日期为当前日期
    ui->updateDateEdit->setDate(QDate::currentDate());
    ui->updateDateEdit->setReadOnly(true);  // 设置为只读
}

MaterialDialog::~MaterialDialog()
{
    delete ui;
}

void MaterialDialog::setMaterialData(const QSqlRecord &record)
{
    // 设置各个字段的数据
    ui->modelComboBox->setCurrentText(record.value("型号").toString());
    ui->descriptionTextEdit->setPlainText(record.value("描述").toString());
    ui->quantitySpinBox->setValue(record.value("数量").toInt());
    ui->unitPriceDoubleSpinBox->setValue(record.value("单价").toDouble());

    // 根据数据库中的值设置 sourceComboBox 的当前索引
    QString selfMadeOrPurchase = record.value("来源").toString();
    int sourceIndex = ui->sourceComboBox->findData(selfMadeOrPurchase);
    if (sourceIndex != -1) {
        ui->sourceComboBox->setCurrentIndex(sourceIndex);
    }

    ui->versionLineEdit->setText(record.value("版本号").toString());
    ui->serialNumberLineEdit->setText(record.value("序列号").toString());
    ui->locationLineEdit->setText(record.value("库位号").toString());

    // 根据数据库中的值设置 statusComboBox 的当前索引
    QString status = record.value("状态").toString();
    int statusIndex = ui->statusComboBox->findData(status);
    if (statusIndex != -1) {
        ui->statusComboBox->setCurrentIndex(statusIndex);
    }

    ui->supplierLineEdit->setText(record.value("供应商").toString());
    ui->supplierPartNumberLineEdit->setText(record.value("供应商料号").toString());
    ui->deliveryDateEdit->setDate(record.value("货期").toDate());
    ui->noteTextEdit->setPlainText(record.value("备注").toString());

    // 设置文件数据
    drawingData = record.value("drawing").toByteArray(); // 设置图纸数据
    photoData = record.value("photo").toByteArray(); // 设置实物照片数据
    specificationData = record.value("specification").toByteArray(); // 设置供应商规格书数据

    // 更新界面状态以显示文件上传情况（可选）
    if (!drawingData.isEmpty()) {
        QMessageBox::information(this, "文件状态", "图纸已加载。");
    }
    if (!photoData.isEmpty()) {
        QMessageBox::information(this, "文件状态", "实物照片已加载。");
    }
    if (!specificationData.isEmpty()) {
        QMessageBox::information(this, "文件状态", "供应商规格书已加载。");
    }

    // 检查数据库中是否有物料维护人
    QString maintainerInDb = record.value("物料维护人").toString();
    if (!maintainerInDb.isEmpty()) {
        currentMaintainer = maintainerInDb;
    }
    ui->maintainerLineEdit->setText(currentMaintainer);

    // 设置更新日期为数据库中的日期
    ui->updateDateEdit->setDate(record.value("更新日期").toDate());
}

QSqlRecord MaterialDialog::getMaterialData() const
{
    QSqlRecord record;

    // 先定义字段
    record.append(QSqlField("型号", QMetaType::fromType<QString>()));
    record.append(QSqlField("描述", QMetaType::fromType<QString>()));
    record.append(QSqlField("数量", QMetaType::fromType<int>()));
    record.append(QSqlField("单价", QMetaType::fromType<double>()));
    record.append(QSqlField("来源", QMetaType::fromType<QString>()));
    record.append(QSqlField("版本号", QMetaType::fromType<QString>()));
    record.append(QSqlField("序列号", QMetaType::fromType<QString>()));
    record.append(QSqlField("库位号", QMetaType::fromType<QString>()));
    record.append(QSqlField("状态", QMetaType::fromType<QString>()));
    record.append(QSqlField("供应商", QMetaType::fromType<QString>()));
    record.append(QSqlField("供应商料号", QMetaType::fromType<QString>()));
    record.append(QSqlField("货期", QMetaType::fromType<QDate>()));
    record.append(QSqlField("备注", QMetaType::fromType<QString>()));
    record.append(QSqlField("物料维护人", QMetaType::fromType<QString>()));
    record.append(QSqlField("更新日期", QMetaType::fromType<QDateTime>()));
    record.append(QSqlField("drawing", QMetaType::fromType<QByteArray>()));
    record.append(QSqlField("photo", QMetaType::fromType<QByteArray>()));
    record.append(QSqlField("specification", QMetaType::fromType<QByteArray>()));


    // 然后设置值
    record.setValue("型号", ui->modelComboBox->currentText());
    record.setValue("描述", ui->descriptionTextEdit->toPlainText());
    record.setValue("数量", ui->quantitySpinBox->value());
    record.setValue("单价", ui->unitPriceDoubleSpinBox->value());

    // 获取来源的用户数据（'P' 或 'M'）
    QString selfMadeOrPurchase = ui->sourceComboBox->currentData().toString();
    record.setValue("来源", selfMadeOrPurchase);

    record.setValue("版本号", ui->versionLineEdit->text());
    record.setValue("序列号", ui->serialNumberLineEdit->text());
    record.setValue("库位号", ui->locationLineEdit->text());

    // 获取状态的用户数据（'PP' 或 'FP'）
    QString status = ui->statusComboBox->currentData().toString();
    record.setValue("状态", status);

    record.setValue("供应商", ui->supplierLineEdit->text());
    record.setValue("供应商料号", ui->supplierPartNumberLineEdit->text());
    record.setValue("货期", ui->deliveryDateEdit->date());
    record.setValue("备注", ui->noteTextEdit->toPlainText());
    record.setValue("物料维护人", ui->maintainerLineEdit->text());
    record.setValue("更新日期", ui->updateDateEdit->date());

    // 设置文件数据
    record.setValue("drawing", drawingData);
    record.setValue("photo", photoData);
    record.setValue("specification", specificationData);

    // 调试输出
    qDebug() << "型号:" << record.value("型号").toString();
    qDebug() << "描述:" << record.value("描述").toString();
    qDebug() << "版本号:" << record.value("版本号").toString();
    qDebug() << "来源:" << record.value("来源").toString();
    qDebug() << "状态:" << record.value("状态").toString();

    return record;
}

void MaterialDialog::on_uploadDrawingButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择图纸", "", "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            drawingData = file.readAll();
            QMessageBox::information(this, "上传成功", "图纸已成功上传。");
        } else {
            QMessageBox::critical(this, "上传失败", "无法打开所选文件。");
        }
    }
}

void MaterialDialog::on_downloadDrawingButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "下载图纸", "", "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(drawingData);
            file.close();
            QMessageBox::information(this, "下载成功", "图纸已成功下载。");
        } else {
            QMessageBox::critical(this, "下载失败", "无法保存文件。");
        }
    }
}

void MaterialDialog::on_uploadPhotoButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择实物照片", "", "图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            photoData = file.readAll();
            QMessageBox::information(this, "上传成功", "实物照片已成功上传。");
        } else {
            QMessageBox::critical(this, "上传失败", "无法打开所选文件。");
        }
    }
}

void MaterialDialog::on_downloadPhotoButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "下载实物照片", "", "图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(photoData);
            file.close();
            QMessageBox::information(this, "下载成功", "实物照片已成功下载。");
        } else {
            QMessageBox::critical(this, "下载失败", "无法保存文件。");
        }
    }
}

void MaterialDialog::on_uploadSpecificationButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择供应商规格书", "", "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            specificationData = file.readAll();
            QMessageBox::information(this, "上传成功", "供应商规格书已成功上传。");
        } else {
            QMessageBox::critical(this, "上传失败", "无法打开所选文件。");
        }
    }
}

void MaterialDialog::on_downloadSpecificationButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "下载供应商规格书", "", "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(specificationData);
            file.close();
            QMessageBox::information(this, "下载成功", "供应商规格书已成功下载。");
        } else {
            QMessageBox::critical(this, "下载失败", "无法保存文件。");
        }
    }
}

void MaterialDialog::on_saveButton_clicked()
{
    // 获取用户输入的数据
    QSqlRecord record = getMaterialData();

    // 检查必要的输入项是否为空，使用 trimmed() 去除空白字符
    if (record.value("型号").toString().trimmed().isEmpty() ||
        record.value("描述").toString().trimmed().isEmpty() ||
        record.value("版本号").toString().trimmed().isEmpty()) {
        QMessageBox::warning(this, "保存失败", "请填写所有必填项。");
        return;
    }

    // 创建一个 SQL 查询对象
    QSqlQuery query;
    query.prepare("INSERT INTO Material (model, description, quantity, unit_price, self_made_or_purchase, version, "
                  "serial_number, location_number, status, supplier, supplier_material_number, delivery_period, "
                  "material_maintainer, update_date, remarks, drawing, photo, supplier_specification) "
                  "VALUES (:model, :description, :quantity, :unit_price, :self_made_or_purchase, :version, "
                  ":serial_number, :location_number, :status, :supplier, :supplier_material_number, :delivery_period, "
                  ":material_maintainer, :update_date, :remarks, :drawing, :photo, :supplier_specification) "
                  "ON DUPLICATE KEY UPDATE "
                  "description = VALUES(description), quantity = VALUES(quantity), "
                  "unit_price = VALUES(unit_price), self_made_or_purchase = VALUES(self_made_or_purchase), version = VALUES(version), "
                  "serial_number = VALUES(serial_number), location_number = VALUES(location_number), status = VALUES(status), "
                  "supplier = VALUES(supplier), supplier_material_number = VALUES(supplier_material_number), "
                  "delivery_period = VALUES(delivery_period), material_maintainer = VALUES(material_maintainer), "
                  "update_date = VALUES(update_date), remarks = VALUES(remarks), "
                  "drawing = VALUES(drawing), photo = VALUES(photo), supplier_specification = VALUES(supplier_specification)");

    // 绑定数据到查询语句
    query.bindValue(":model", record.value("型号").toString());
    query.bindValue(":description", record.value("描述").toString());
    query.bindValue(":quantity", record.value("数量").toInt());
    query.bindValue(":unit_price", record.value("单价").toDouble());
    query.bindValue(":self_made_or_purchase", record.value("来源").toString());
    query.bindValue(":version", record.value("版本号").toString());
    query.bindValue(":serial_number", record.value("序列号").toString());
    query.bindValue(":location_number", record.value("库位号").toString());
    query.bindValue(":status", record.value("状态").toString());
    query.bindValue(":supplier", record.value("供应商").toString());
    query.bindValue(":supplier_material_number", record.value("供应商料号").toString());
    query.bindValue(":delivery_period", record.value("货期").toDate());
    query.bindValue(":material_maintainer", record.value("物料维护人").toString());
    query.bindValue(":update_date", record.value("更新日期").toDate());
    query.bindValue(":remarks", record.value("备注").toString());

    // 绑定文件数据到查询语句
    query.bindValue(":drawing", record.value("drawing").toByteArray());
    query.bindValue(":photo", record.value("photo").toByteArray());
    query.bindValue(":supplier_specification", record.value("specification").toByteArray());

    // 执行查询并检查结果
    if (!query.exec()) {
        QMessageBox::critical(this, "保存失败", "无法保存数据到数据库。\n错误信息: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "保存成功", "物料信息已成功保存！");
        accept();  // 关闭对话框
    }
}
