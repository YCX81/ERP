// materialdialog.cpp

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
#include <QStandardPaths>
#include <QDir>
#include <QUuid>

// 定义图纸和照片的上传目录
const QString DRAWING_DIR = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/drawings/";
const QString PHOTO_DIR = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/photos/";


MaterialDialog::MaterialDialog(const QString &currentUser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialDialog),
    currentMaintainer(currentUser),
    defectiveQuantityEditingEnabled(false),
    drawingPath(""),
    photoPath("")
{
    ui->setupUi(this);

    // 确保上传目录存在
    QDir().mkpath(DRAWING_DIR);
    QDir().mkpath(PHOTO_DIR);

    // 注册 CategoryInfo 类型
    qRegisterMetaType<CategoryInfo>("CategoryInfo");

    // 初始化类别下拉框
    ui->categoryComboBox->clear();
    QSqlQuery query("SELECT category_code, category_name, letter_code FROM Categories ORDER BY category_code");
    while (query.next()) {
        int code = query.value("category_code").toInt();
        QString name = query.value("category_name").toString();
        QString letterCode = query.value("letter_code").toString();
        CategoryInfo category = { code, name, letterCode };
        QString displayText = QString("%1 - %2").arg(name).arg(letterCode);
        ui->categoryComboBox->addItem(displayText, QVariant::fromValue(category));
    }

    // 设置来源下拉框
    ui->sourceComboBox->clear();
    ui->sourceComboBox->addItem("自制 (P)", QVariant("P"));
    ui->sourceComboBox->addItem("采购 (M)", QVariant("M"));

    // 初始化供应商下拉框
    ui->supplierComboBox->clear();
    QSqlQuery supplierQuery("SELECT supplier_id, supplier_name FROM Suppliers ORDER BY supplier_id");
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
        ui->goodQuantitySpinBox->setVisible(false);
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
        ui->generateMaterialNumberButton->setVisible(false);
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
        ui->goodQuantitySpinBox->setVisible(true);
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
        ui->generateMaterialNumberButton->setVisible(true);
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
        int categoryCode = record.value("category_code").toInt();

        // 遍历 categoryComboBox，找到匹配的 category_code 并设置当前索引
        int index = -1;
        for (int i = 0; i < ui->categoryComboBox->count(); ++i) {
            QVariant data = ui->categoryComboBox->itemData(i);
            if (data.isValid()) {
                CategoryInfo category = data.value<CategoryInfo>();
                if (category.category_code == categoryCode) {
                    index = i;
                    break;
                }
            }
        }
        if (index != -1) {
            ui->categoryComboBox->setCurrentIndex(index);
        } else {
            // 如果未找到匹配项，可以选择设置为第一个或提示用户
            ui->categoryComboBox->setCurrentIndex(0);
            QMessageBox::warning(this, "警告", "未找到匹配的物料类别。");
        }

        // 设置其他字段
        ui->descriptionTextEdit->setPlainText(record.value("description").toString());
        ui->goodQuantitySpinBox->setValue(record.value("good_quantity").toInt());
        ui->unitPriceDoubleSpinBox->setValue(record.value("unit_price").toDouble());

        // 设置来源
        QString selfMadeOrPurchase = record.value("self_made_or_purchase").toString();
        int sourceIndex = ui->sourceComboBox->findData(selfMadeOrPurchase);
        if (sourceIndex != -1) {
            ui->sourceComboBox->setCurrentIndex(sourceIndex);
        }

        ui->versionSpinBox->setValue(record.value("version").toInt());
        ui->serialNumberLineEdit->setText(record.value("serial_number").toString());
        ui->locationLineEdit->setText(record.value("location_number").toString());

        // 设置供应商
        QString supplierID = record.value("supplier_id").toString();
        int supplierIndex = ui->supplierComboBox->findData(supplierID);
        if (supplierIndex != -1) {
            ui->supplierComboBox->setCurrentIndex(supplierIndex);
        }

        ui->supplierMaterialNumberLineEdit->setText(record.value("supplier_material_number").toString());
        ui->deliverySpinBox->setValue(record.value("delivery_period").toInt());
        ui->noteTextEdit->setPlainText(record.value("remarks").toString());

        // 设置不良品数量
        ui->defectiveQuantitySpinBox->setValue(record.value("defective_quantity").toInt());

        // 设置文件路径
        drawingPath = record.value("drawing").toString();
        photoPath = record.value("photo").toString();

        // 更新界面状态以显示文件上传情况（可选）
        if (!drawingPath.isEmpty()) {
            ui->drawingStatusLabel->setText("图纸已上传");
        } else {
            ui->drawingStatusLabel->setText("无图纸");
        }
        if (!photoPath.isEmpty()) {
            ui->photoStatusLabel->setText("照片已上传");
        } else {
            ui->photoStatusLabel->setText("无照片");
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
}

QSqlRecord MaterialDialog::getMaterialData() const
{
    QSqlRecord record;

    // 定义字段，使用与数据库一致的英文字段名
    record.append(QSqlField("category_code",QVariant::Int));
    record.append(QSqlField("category_name", QVariant::String));
    record.append(QSqlField("material_number", QVariant::String));
    record.append(QSqlField("description", QVariant::String));
    record.append(QSqlField("unit_price", QVariant::Double));
    record.append(QSqlField("self_made_or_purchase", QVariant::String));
    record.append(QSqlField("version", QVariant::Int)); // 从 "version_number" 更改为 "version"
    record.append(QSqlField("serial_number", QVariant::String));
    record.append(QSqlField("location_number", QVariant::String));
    record.append(QSqlField("good_quantity", QVariant::Int));
    record.append(QSqlField("defective_quantity", QVariant::Int));
    record.append(QSqlField("supplier_id", QVariant::String)); // 从 "supplier" 更改为 "supplier_id"
    record.append(QSqlField("supplier_material_number", QVariant::String));
    record.append(QSqlField("delivery_period", QVariant::Int));
    record.append(QSqlField("remarks", QVariant::String));
    record.append(QSqlField("material_maintainer", QVariant::String));
    record.append(QSqlField("update_date", QVariant::DateTime));
    record.append(QSqlField("drawing", QVariant::ByteArray));
    record.append(QSqlField("photo", QVariant::ByteArray));

    QVariant categoryData = ui->categoryComboBox->currentData();
    if (categoryData.isValid()) {
        CategoryInfo category = categoryData.value<CategoryInfo>();
        qDebug() << "Selected Category Code:" << category.category_code << "Category Name:" << category.category_name;
        record.setValue("category_code", category.category_code);
        record.setValue("category_name", category.category_name); // 新增
    } else {
        qDebug() << "No valid category data selected.";
        record.setValue("category_code", QVariant());
        record.setValue("category_name", QVariant()); // 新增
    }

    // 设置值
    record.setValue("material_number", ui->materialNumberLineEdit->text());
    // 不需要设置 "category"，因为我们已经设置了 "category_code"
    record.setValue("description", ui->descriptionTextEdit->toPlainText());
    // 移除 "quantity" 字段设置，因为数据库中没有单独的 "quantity"
    record.setValue("unit_price", ui->unitPriceDoubleSpinBox->value());

    QString selfMadeOrPurchase = ui->sourceComboBox->currentData().toString();
    record.setValue("self_made_or_purchase", selfMadeOrPurchase);

    record.setValue("version", ui->versionSpinBox->value()); // 从 "version_number" 更改为 "version"

    record.setValue("serial_number", ui->serialNumberLineEdit->text());
    record.setValue("location_number", ui->locationLineEdit->text());

    // 设置良品数量和不良品数量
    record.setValue("good_quantity", ui->goodQuantitySpinBox->value());
    record.setValue("defective_quantity", ui->defectiveQuantitySpinBox->value());

    // 设置供应商
    QString supplierID = ui->supplierComboBox->currentData().toString();
    record.setValue("supplier_id", supplierID); // 从 "supplier" 更改为 "supplier_id"
    record.setValue("supplier_material_number", ui->supplierMaterialNumberLineEdit->text());
    record.setValue("delivery_period", ui->deliverySpinBox->value());
    record.setValue("remarks", ui->noteTextEdit->toPlainText());
    record.setValue("material_maintainer", ui->maintainerLineEdit->text());
    record.setValue("update_date", ui->updateDateTimeEdit->dateTime());

    // 设置文件路径
    record.setValue("drawing", drawingPath);
    record.setValue("photo", photoPath);

    return record;
}

void MaterialDialog::on_generateMaterialNumberButton_clicked()
{
    qDebug() << "Generate Material Number button clicked.";

    // 获取类别信息
    QVariant categoryData = ui->categoryComboBox->currentData();
    if (!categoryData.isValid()) {
        QMessageBox::warning(this, "警告", "请选择物料类别。");
        return;
    }
    CategoryInfo category = categoryData.value<CategoryInfo>();
    QString categoryLetterCode = category.letter_code;

    // 获取供应商ID
    QString supplierID = ui->supplierComboBox->currentData().toString();
    if (supplierID.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择供应商。");
        return;
    }

    // 获取供应商名称
    QString displayText = ui->supplierComboBox->currentText(); // 格式为 "supplierID - supplierName"
    QStringList parts = displayText.split(" - ");
    QString supplierName = parts.size() > 1 ? parts[1].trimmed() : "";

    qDebug() << "Selected Supplier ID:" << supplierID;
    qDebug() << "Selected Supplier Name:" << supplierName;

    // 检查供应商是否为 P&G
    if (supplierName.compare("P&G", Qt::CaseInsensitive) == 0) {
        QString supplierMaterialNumber = ui->supplierMaterialNumberLineEdit->text().trimmed();
        if (supplierMaterialNumber.isEmpty()) {
            QMessageBox::warning(this, "警告", "供应商料号为空，无法生成物料号。");
            return;
        }

        ui->materialNumberLineEdit->setText(supplierMaterialNumber);
        QMessageBox::information(this, "生成成功", "物料号已设为供应商料号：" + supplierMaterialNumber);
        return;
    }

    // 如果供应商不是 P&G，则按照原逻辑生成物料号

    // 获取 MK 号
    QString mkNumber = ui->mkNumberComboBox->currentText(); // 例如 "MK1"
    if (mkNumber.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择 MK 号。");
        return;
    }

    // 生成三位自动编号，确保不重复
    QSqlQuery query;
    // 假设 material_number 格式为 [categoryLetterCode][supplierID][autoGeneratedCode][mkNumber]
    // 例如: CATSUP001MK1
    // 需要根据实际格式调整 SUBSTRING 的位置和长度

    // 构建匹配模式，确保包含当前的 MK 号
    QString pattern = QString("%1%2___%3%").arg(categoryLetterCode).arg(supplierID).arg(mkNumber);
    int startPosition = categoryLetterCode.length() + supplierID.length() + 1; // +1 因为 SUBSTRING 的位置从1开始

    qDebug() << "Pattern:" << pattern;
    qDebug() << "Start Position:" << startPosition;

    query.prepare("SELECT MAX(CAST(SUBSTRING(material_number, :start, 3) AS UNSIGNED)) AS max_code "
                  "FROM Material WHERE material_number LIKE :pattern");
    query.bindValue(":start", startPosition);
    query.bindValue(":pattern", pattern);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "查询数据库失败: " + query.lastError().text());
        qDebug() << "SQL Error:" << query.lastError().text();
        return;
    }

    int maxCode = 0;
    if (query.next()) {
        maxCode = query.value("max_code").toInt();
    }
    qDebug() << "Max Code:" << maxCode;

    int newCode = maxCode + 1;
    if (newCode > 999) {
        QMessageBox::critical(this, "错误", "自动编号已达到最大值 (999)。无法生成新的物料号。");
        return;
    }
    QString autoGeneratedCode = QString::number(newCode).rightJustified(3, '0');
    qDebug() << "Auto Generated Code:" << autoGeneratedCode;

    // 生成物料号
    QString materialNumber = QString("%1%2%3%4")
                                 .arg(categoryLetterCode)
                                 .arg(supplierID)
                                 .arg(autoGeneratedCode)
                                 .arg(mkNumber);
    qDebug() << "Generated Material Number:" << materialNumber;

    ui->materialNumberLineEdit->setText(materialNumber);
    QMessageBox::information(this, "生成成功", "物料号已成功生成：" + materialNumber);
}

QString uniqueFileName(const QFileInfo &fileInfo) {
    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    return QString("%1_%2").arg(uuid, fileInfo.fileName());
}

void MaterialDialog::on_uploadDrawingButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择图纸", "", "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString uniqueName = uniqueFileName(fileInfo);
        QString destPath = DRAWING_DIR + uniqueName;

        if (QFile::copy(filePath, destPath)) {
            drawingPath = destPath;
            ui->drawingStatusLabel->setText("图纸已上传");
            QMessageBox::information(this, "上传成功", "图纸已成功上传。");
        } else {
            QMessageBox::critical(this, "上传失败", "无法复制图纸到上传目录。");
        }
    }
}



void MaterialDialog::on_downloadDrawingButton_clicked()
{
    if (drawingPath.isEmpty()) {
        QMessageBox::information(this, "提示", "没有上传的图纸可供下载。");
        return;
    }

    QString defaultFileName = QFileInfo(drawingPath).fileName();
    QString filePath = QFileDialog::getSaveFileName(this, "下载图纸", defaultFileName, "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        if (QFile::copy(drawingPath, filePath)) {
            QMessageBox::information(this, "下载成功", "图纸已成功下载。");
        } else {
            QMessageBox::critical(this, "下载失败", "无法复制图纸到指定位置。");
        }
    }
}


void MaterialDialog::on_uploadPhotoButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择实物照片", "", "图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString uniqueName = uniqueFileName(fileInfo);
        QString destPath = PHOTO_DIR + uniqueName;

        if (QFile::copy(filePath, destPath)) {
            photoPath = destPath;
            ui->photoStatusLabel->setText("照片已上传");
            QMessageBox::information(this, "上传成功", "实物照片已成功上传。");
        } else {
            QMessageBox::critical(this, "上传失败", "无法复制实物照片到上传目录。");
        }
    }
}


void MaterialDialog::on_downloadPhotoButton_clicked()
{
    if (photoPath.isEmpty()) {
        QMessageBox::information(this, "提示", "没有上传的实物照片可供下载。");
        return;
    }

    QString defaultFileName = QFileInfo(photoPath).fileName();
    QString filePath = QFileDialog::getSaveFileName(this, "下载实物照片", defaultFileName, "图片文件 (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        if (QFile::copy(photoPath, filePath)) {
            QMessageBox::information(this, "下载成功", "实物照片已成功下载。");
        } else {
            QMessageBox::critical(this, "下载失败", "无法复制实物照片到指定位置。");
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
    QString materialNumber = record.value("material_number").toString();
    QString mkNumber = ui->mkNumberComboBox->currentText();

    // 检查数据库中是否存在相同的物料号
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM Material WHERE material_number = :material_number");
    checkQuery.bindValue(":material_number", materialNumber);

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "错误", "无法检查物料号是否存在。\n错误信息: " + checkQuery.lastError().text());
        return;
    }

    bool isUpdate = checkQuery.next(); // 如果有结果，说明需要更新

    // 创建一个 SQL 查询对象
    QSqlQuery query;
    if (!isUpdate) {
        // 新增物料
        query.prepare("INSERT INTO Material (material_number, category_code, category_name, description, unit_price, self_made_or_purchase, version, "
                      "serial_number, location_number, good_quantity, defective_quantity, supplier_id, supplier_material_number, "
                      "delivery_period, material_maintainer, update_date, remarks, drawing, photo) "
                      "VALUES (:material_number, :category_code, :category_name, :description, :unit_price, :self_made_or_purchase, :version, "
                      ":serial_number, :location_number, :good_quantity, :defective_quantity, :supplier_id, :supplier_material_number, "
                      ":delivery_period, :material_maintainer, :update_date, :remarks, :drawing, :photo)");
    } else {
        // 更新物料
        query.prepare("UPDATE Material SET category_code = :category_code, category_name = :category_name, "
                      "description = :description, unit_price = :unit_price, self_made_or_purchase = :self_made_or_purchase, "
                      "version = :version, serial_number = :serial_number, location_number = :location_number, "
                      "good_quantity = :good_quantity, defective_quantity = :defective_quantity, supplier_id = :supplier_id, "
                      "supplier_material_number = :supplier_material_number, delivery_period = :delivery_period, "
                      "material_maintainer = :material_maintainer, update_date = :update_date, remarks = :remarks, "
                      "drawing = :drawing, photo = :photo WHERE material_number = :material_number");
    }

    // 绑定数据到查询语句
    query.bindValue(":material_number", record.value("material_number"));
    query.bindValue(":category_code", record.value("category_code"));
    query.bindValue(":category_name", record.value("category_name")); // 新增
    query.bindValue(":description", record.value("description"));
    query.bindValue(":unit_price", record.value("unit_price"));
    query.bindValue(":self_made_or_purchase", record.value("self_made_or_purchase"));
    query.bindValue(":version", record.value("version"));
    query.bindValue(":serial_number", record.value("serial_number"));
    query.bindValue(":location_number", record.value("location_number"));
    query.bindValue(":good_quantity", record.value("good_quantity"));
    query.bindValue(":defective_quantity", record.value("defective_quantity"));
    query.bindValue(":supplier_id", record.value("supplier_id"));
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
        qDebug() << "SQL Error:" << query.lastError().text(); // 添加调试信息
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
        ui->versionSpinBox->value() == 0) { // 使用 value() 而不是 text()
        QMessageBox::warning(this, "保存失败", "请填写所有必填项。");
        return false;
    }
    return true;
}
