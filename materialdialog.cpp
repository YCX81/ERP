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
#include <QDateTime>

MaterialDialog::MaterialDialog(const QString &currentUser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialDialog),
    currentMaintainer(currentUser),
    defectiveQuantityEditingEnabled(false)
{
    ui->setupUi(this);

    // 注册 CategoryInfo 类型
    qRegisterMetaType<CategoryInfo>("CategoryInfo");

    // 初始化类别列表
    QList<CategoryInfo> categoryList = {
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

    // 初始化类别下拉框
    ui->categoryComboBox->clear();
    for (const CategoryInfo &category : categoryList) {
        QString displayText = QString("%1 - %2").arg(category.name).arg(category.letterCode);
        ui->categoryComboBox->addItem(displayText, QVariant::fromValue(category));
    }

    // 设置 sourceComboBox 的选项和用户数据
    ui->sourceComboBox->clear();
    ui->sourceComboBox->addItem("自制 (P)", QVariant("P"));
    ui->sourceComboBox->addItem("采购 (M)", QVariant("M"));

    // 初始化供应商下拉框
    ui->supplierComboBox->clear();
    QSqlQuery supplierQuery("SELECT supplier_id, supplier_name FROM Suppliers");
    while (supplierQuery.next()) {
        QString supplierID = supplierQuery.value("supplier_id").toString();
        QString supplierName = supplierQuery.value("supplier_name").toString();
        QString displayText = QString("%1 - %2").arg(supplierID, supplierName);
        ui->supplierComboBox->addItem(displayText, supplierID);
    }


    // 设置物料维护人为当前用户名
    ui->maintainerLineEdit->setText(currentMaintainer);
    ui->maintainerLineEdit->setReadOnly(true);  // 设置为只读

    // 设置初始更新日期为当前日期和时间（精确到小时分钟）
    ui->updateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->updateDateTimeEdit->setReadOnly(true);  // 设置为只读
}

MaterialDialog::~MaterialDialog()
{
    delete ui;
}

void MaterialDialog::enableDefectiveQuantityEditing(bool enable)
{
    defectiveQuantityEditingEnabled = enable;

    if (enable) {
        // 仅显示与不良品数量相关的控件，隐藏其他控件
        ui->defectiveQuantityLabel->setVisible(true);
        ui->defectiveQuantitySpinBox->setVisible(true);

        ui->goodQuantityLabel->setVisible(false);
        ui->goodQuantitySpinBox->setVisible(false);
        // 隐藏其他不需要的控件
        ui->categoryComboBox->setVisible(false);
        ui->descriptionTextEdit->setVisible(false);
        ui->quantitySpinBox->setVisible(false);
        ui->unitPriceDoubleSpinBox->setVisible(false);
        ui->sourceComboBox->setVisible(false);
        ui->versionSpinBox->setVisible(false);
        ui->serialNumberLineEdit->setVisible(false);
        ui->locationLineEdit->setVisible(false);
        ui->supplierComboBox->setVisible(false);
        ui->supplierMaterialNumberLineEdit->setVisible(false);
        ui->noteTextEdit->setVisible(false);
        ui->uploadDrawingButton->setVisible(false);
        ui->downloadDrawingButton->setVisible(false);
        ui->uploadPhotoButton->setVisible(false);
        ui->downloadPhotoButton->setVisible(false);
        ui->materialNumberGenerateButton->setVisible(false);
        ui->deliverySpinBox->setVisible(false);
        ui->deliveryLabel->setVisible(false);
        ui->materialNumberLabel->setVisible(false);
        ui->materialNumberLineEdit->setVisible(false);
        ui->drawingStatusLabel->setVisible(false);
        ui->photoStatusLabel->setVisible(false);
        ui->maintainerLabel->setVisible(false);
        ui->maintainerLineEdit->setVisible(false);
        ui->updateDateLabel->setVisible(false);
        ui->updateDateTimeEdit->setVisible(false);
    } else {
        // 显示所有控件
        ui->defectiveQuantityLabel->setVisible(true);
        ui->defectiveQuantitySpinBox->setVisible(true);

        ui->goodQuantityLabel->setVisible(true);
        ui->goodQuantitySpinBox->setVisible(true);
        ui->categoryComboBox->setVisible(true);
        ui->descriptionTextEdit->setVisible(true);
        ui->quantitySpinBox->setVisible(true);
        ui->unitPriceDoubleSpinBox->setVisible(true);
        ui->sourceComboBox->setVisible(true);
        ui->versionSpinBox->setVisible(true);
        ui->serialNumberLineEdit->setVisible(true);
        ui->locationLineEdit->setVisible(true);
        ui->supplierComboBox->setVisible(true);
        ui->supplierMaterialNumberLineEdit->setVisible(true);
        ui->noteTextEdit->setVisible(true);
        ui->uploadDrawingButton->setVisible(true);
        ui->downloadDrawingButton->setVisible(true);
        ui->uploadPhotoButton->setVisible(true);
        ui->downloadPhotoButton->setVisible(true);
        ui->materialNumberGenerateButton->setVisible(true);
        ui->deliverySpinBox->setVisible(true);
        ui->deliveryLabel->setVisible(true);
        ui->materialNumberLabel->setVisible(true);
        ui->materialNumberLineEdit->setVisible(true);
        //ui->drawingStatusLabel->setVisible(true);
        //ui->photoStatusLabel->setVisible(true);
        ui->maintainerLabel->setVisible(true);
        ui->maintainerLineEdit->setVisible(true);
        ui->updateDateLabel->setVisible(true);
        ui->updateDateTimeEdit->setVisible(true);
    }
}

void MaterialDialog::setMaterialData(const QSqlRecord &record)
{
    if (!defectiveQuantityEditingEnabled) {
        ui->categoryComboBox->setCurrentText(record.value("model").toString());
        ui->descriptionTextEdit->setPlainText(record.value("description").toString());
        ui->quantitySpinBox->setValue(record.value("quantity").toInt());
        ui->unitPriceDoubleSpinBox->setValue(record.value("unit_price").toDouble());

        // 设置来源
        QString selfMadeOrPurchase = record.value("self_made_or_purchase").toString();
        int sourceIndex = ui->sourceComboBox->findData(selfMadeOrPurchase);
        if (sourceIndex != -1) {
            ui->sourceComboBox->setCurrentIndex(sourceIndex);
        }

        ui->versionSpinBox->setValue(record.value("version_number").toInt());
        ui->serialNumberLineEdit->setText(record.value("serial_number").toString());
        ui->locationLineEdit->setText(record.value("location_number").toString());

        // 设置良品数量
        ui->goodQuantitySpinBox->setValue(record.value("good_quantity").toInt());
    }

    // 设置不良品数量
    ui->defectiveQuantitySpinBox->setValue(record.value("defective_quantity").toInt());

    // 设置供应商
    QString supplierID = record.value("supplier").toString();
    int supplierIndex = ui->supplierComboBox->findData(supplierID);
    if (supplierIndex != -1) {
        ui->supplierComboBox->setCurrentIndex(supplierIndex);
    }

    ui->supplierMaterialNumberLineEdit->setText(record.value("supplier_material_number").toString());
    ui->deliverySpinBox->setValue(record.value("delivery_period").toInt());
    ui->noteTextEdit->setPlainText(record.value("remarks").toString());

    // 设置文件数据
    drawingData = record.value("drawing").toByteArray();
    photoData = record.value("photo").toByteArray();

    // 更新界面状态以显示文件上传情况（可选）
    if (!drawingData.isEmpty()) {
        ui->drawingStatusLabel->setText("图纸已加载");
    }
    if (!photoData.isEmpty()) {
        ui->photoStatusLabel->setText("照片已加载");
    }

    // 设置物料维护人
    QString maintainerInDb = record.value("material_maintainer").toString();
    if (!maintainerInDb.isEmpty()) {
        currentMaintainer = maintainerInDb;
    }
    ui->maintainerLineEdit->setText(currentMaintainer);

    // 设置更新日期为数据库中的日期
    ui->updateDateTimeEdit->setDateTime(record.value("update_date").toDateTime());

    // 设置物料号
    ui->materialNumberLineEdit->setText(record.value("material_number").toString());
}

QSqlRecord MaterialDialog::getMaterialData() const
{
    QSqlRecord record;

    // 定义字段，使用与数据库一致的英文字段名
    record.append(QSqlField("material_number", QVariant::String));
    //record.append(QSqlField("model", QVariant::String));
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
    record.append(QSqlField("remarks", QVariant::String));
    record.append(QSqlField("material_maintainer", QVariant::String));
    record.append(QSqlField("update_date", QVariant::DateTime));
    record.append(QSqlField("drawing", QVariant::ByteArray));
    record.append(QSqlField("photo", QVariant::ByteArray));

    // 设置值
    record.setValue("material_number", ui->materialNumberLineEdit->text());
    record.setValue("category", ui->categoryComboBox->currentText());
    record.setValue("description", ui->descriptionTextEdit->toPlainText());
    record.setValue("quantity", ui->quantitySpinBox->value());
    record.setValue("unit_price", ui->unitPriceDoubleSpinBox->value());

    QString selfMadeOrPurchase = ui->sourceComboBox->currentData().toString();
    record.setValue("self_made_or_purchase", selfMadeOrPurchase);

    record.setValue("version_number", ui->versionSpinBox->value());
    record.setValue("serial_number", ui->serialNumberLineEdit->text());
    record.setValue("location_number", ui->locationLineEdit->text());

    // 设置良品数量和不良品数量
    record.setValue("good_quantity", ui->goodQuantitySpinBox->value());
    record.setValue("defective_quantity", ui->defectiveQuantitySpinBox->value());

    // 设置供应商
    QString supplierID = ui->supplierComboBox->currentData().toString();
    record.setValue("supplier_id", supplierID);

    record.setValue("supplier_material_number", ui->supplierMaterialNumberLineEdit->text());
    record.setValue("delivery_period", ui->deliverySpinBox->value());
    record.setValue("remarks", ui->noteTextEdit->toPlainText());
    record.setValue("material_maintainer", ui->maintainerLineEdit->text());
    record.setValue("update_date", ui->updateDateTimeEdit->dateTime());

    // 设置文件数据
    record.setValue("drawing", drawingData);
    record.setValue("photo", photoData);

    return record;
}

void MaterialDialog::on_materialNumberGenerateButton_clicked()
{
    // 获取类别信息
    QVariant categoryData = ui->categoryComboBox->currentData();
    if (!categoryData.isValid()) {
        QMessageBox::warning(this, "警告", "请选择物料类别。");
        return;
    }
    CategoryInfo category = categoryData.value<CategoryInfo>();
    QString categoryLetterCode = category.letterCode;

    // 获取供应商ID
    QString supplierID = ui->supplierComboBox->currentData().toString();

    // 获取版本号
    int versionNumber = ui->versionSpinBox->value();

    // 生成三位自动编号，确保不重复
    QSqlQuery query;
    query.prepare("SELECT MAX(CAST(SUBSTRING(material_number, 5, 3) AS UNSIGNED)) FROM Material WHERE material_number LIKE :pattern");
    QString pattern = QString("%1%2___%").arg(categoryLetterCode).arg(supplierID);
    query.bindValue(":pattern", pattern);
    int maxCode = 0;
    if (query.exec() && query.next()) {
        maxCode = query.value(0).toInt();
    }
    int newCode = maxCode + 1;
    QString autoGeneratedCode = QString("%1").arg(newCode, 3, 10, QChar('0'));

    // 生成物料号
    QString materialNumber = QString("%1%2%3").arg(categoryLetterCode).arg(supplierID).arg(autoGeneratedCode);

    // 如果版本号不为 0，则添加 "MK" 和版本号
    if (versionNumber != 0) {
        materialNumber += QString("MK%1").arg(versionNumber);
    }

    ui->materialNumberLineEdit->setText(materialNumber);
}


void MaterialDialog::on_uploadDrawingButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择图纸", "", "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            drawingData = file.readAll();
            ui->drawingStatusLabel->setText("图纸已上传");
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
            ui->photoStatusLabel->setText("照片已上传");
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

void MaterialDialog::on_saveButton_clicked()
{
    // 数据验证
    if (!validateInput()) {
        return;
    }

    // 获取用户输入的数据
    QSqlRecord record = getMaterialData();

    // 创建一个 SQL 查询对象
    QSqlQuery query;
    if (record.value("id").isNull()) {
        // 新增物料
        query.prepare("INSERT INTO Material (material_number,  description, quantity, unit_price, self_made_or_purchase, version, "
                      "serial_number, location_number, good_quantity, defective_quantity, supplier, supplier_material_number, "
                      "delivery_period, material_maintainer, update_date, remarks, drawing, photo) "
                      "VALUES (:material_number, :description, :quantity, :unit_price, :self_made_or_purchase, :version, "
                      ":serial_number, :location_number, :good_quantity, :defective_quantity, :supplier, "
                      ":supplier_material_number, :delivery_period, :material_maintainer, :update_date, :remarks, "
                      ":drawing, :photo)");
    } else {
        // 更新物料
        query.prepare("UPDATE Material SET material_number = :material_number,  description = :description, quantity = :quantity, "
                      "unit_price = :unit_price, self_made_or_purchase = :self_made_or_purchase, version = :version, "
                      "serial_number = :serial_number, location_number = :location_number, good_quantity = :good_quantity, "
                      "defective_quantity = :defective_quantity, supplier = :supplier, supplier_material_number = :supplier_material_number, "
                      "delivery_period = :delivery_period, material_maintainer = :material_maintainer, update_date = :update_date, "
                      "remarks = :remarks, drawing = :drawing, photo = :photo WHERE id = :id");
        query.bindValue(":id", record.value("id"));
    }

    // 绑定数据到查询语句
    query.bindValue(":material_number", record.value("material_number"));
    //query.bindValue(":model", record.value("model"));
    query.bindValue(":description", record.value("description"));
    query.bindValue(":quantity", record.value("quantity"));
    query.bindValue(":unit_price", record.value("unit_price"));
    query.bindValue(":self_made_or_purchase", record.value("self_made_or_purchase"));
    query.bindValue(":version", record.value("version"));
    query.bindValue(":serial_number", record.value("serial_number"));
    query.bindValue(":location_number", record.value("location_number"));
    query.bindValue(":good_quantity", record.value("good_quantity"));
    query.bindValue(":defective_quantity", record.value("defective_quantity"));
    query.bindValue(":supplier", record.value("supplier"));
    query.bindValue(":supplier_material_number", record.value("supplier_material_number"));
    query.bindValue(":delivery_period", record.value("delivery_period"));
    query.bindValue(":material_maintainer", record.value("material_maintainer"));
    query.bindValue(":update_date", record.value("update_date"));
    query.bindValue(":remarks", record.value("remarks"));
    query.bindValue(":drawing", record.value("drawing"));
    query.bindValue(":photo", record.value("photo"));

    // 执行查询并检查结果
    if (!query.exec()) {
        QMessageBox::critical(this, "保存失败", "无法保存数据到数据库。\n错误信息: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "保存成功", "物料信息已成功保存！");
        accept();  // 关闭对话框
    }
}

bool MaterialDialog::validateInput()
{
    // 检查必要的输入项是否为空，使用 trimmed() 去除空白字符
    if (ui->categoryComboBox->currentText().trimmed().isEmpty() ||
        ui->descriptionTextEdit->toPlainText().trimmed().isEmpty() ||
        ui->versionSpinBox->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "保存失败", "请填写所有必填项。");
        return false;
    }
    return true;
}
