#include "orderdialog.h"
#include "ui_orderdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QDateTime>

OrderDialog::OrderDialog(const QString &currentUser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderDialog),
    currentUser(currentUser)
{
    ui->setupUi(this);

    // 初始化状态下拉框
    ui->statusComboBox->addItems(QStringList() << "预下单" << "缺料" << "制作中" << "制作完毕" << "已出库");

    // 初始化成品列表
    ui->productTableWidget->setColumnCount(4);
    QStringList headers;
    headers << "成品型号" << "描述" << "数量" << "状态";
    ui->productTableWidget->setHorizontalHeaderLabels(headers);
    ui->productTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->productTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

OrderDialog::~OrderDialog()
{
    delete ui;
}

void OrderDialog::setOrderData(const QSqlRecord &record)
{
    orderRecord = record;

    ui->orderNumberLineEdit->setText(record.value("order_number").toString());
    ui->customerNameLineEdit->setText(record.value("customer_name").toString());
    ui->statusComboBox->setCurrentText(record.value("status").toString());
    ui->remarksTextEdit->setPlainText(record.value("remarks").toString());

    // 加载订单成品
    int orderId = record.value("id").toInt();
    QSqlQuery query;
    query.prepare("SELECT product_id, quantity, status FROM Order_Product WHERE order_id = :order_id");
    query.bindValue(":order_id", orderId);
    if (query.exec()) {
        while (query.next()) {
            int productId = query.value("product_id").toInt();
            int quantity = query.value("quantity").toInt();
            QString status = query.value("status").toString();

            QSqlQuery productQuery;
            productQuery.prepare("SELECT model, description FROM Product WHERE id = :id");
            productQuery.bindValue(":id", productId);
            if (productQuery.exec() && productQuery.next()) {
                QString model = productQuery.value("model").toString();
                QString description = productQuery.value("description").toString();

                OrderProduct op = {productId, model, description, quantity, status};
                orderProducts.append(op);
            }
        }
        updateProductTable();
    }
}

QSqlRecord OrderDialog::getOrderData() const
{
    QSqlRecord record = orderRecord;

    record.setValue("order_number", ui->orderNumberLineEdit->text().trimmed());
    record.setValue("customer_name", ui->customerNameLineEdit->text().trimmed());
    record.setValue("status", ui->statusComboBox->currentText());
    record.setValue("remarks", ui->remarksTextEdit->toPlainText().trimmed());

    return record;
}

void OrderDialog::on_saveButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    if (orderProducts.isEmpty()) {
        QMessageBox::warning(this, "警告", "请添加成品。");
        return;
    }

    QSqlRecord record = getOrderData();

    QSqlQuery query;
    if (record.value("id").isNull() || record.value("id").toInt() == 0) {
        // 新增订单
        query.prepare("INSERT INTO `Order` (order_number, customer_name, status, remarks, created_at, updated_at) "
                      "VALUES (:order_number, :customer_name, :status, :remarks, NOW(), NOW())");
    } else {
        // 更新订单
        query.prepare("UPDATE `Order` SET order_number = :order_number, customer_name = :customer_name, "
                      "status = :status, remarks = :remarks, updated_at = NOW() WHERE id = :id");
        query.bindValue(":id", record.value("id"));
    }
    query.bindValue(":order_number", record.value("order_number"));
    query.bindValue(":customer_name", record.value("customer_name"));
    query.bindValue(":status", record.value("status"));
    query.bindValue(":remarks", record.value("remarks"));

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "无法保存订单信息：" + query.lastError().text());
        return;
    }

    int orderId = record.value("id").toInt();
    if (orderId == 0) {
        // 获取新插入订单的ID
        orderId = query.lastInsertId().toInt();
    }

    // 删除旧的订单成品
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM Order_Product WHERE order_id = :order_id");
    deleteQuery.bindValue(":order_id", orderId);
    if (!deleteQuery.exec()) {
        QMessageBox::warning(this, "警告", "无法删除旧的订单成品：" + deleteQuery.lastError().text());
    }

    // 插入新的订单成品
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO Order_Product (order_id, product_id, quantity, status) "
                        "VALUES (:order_id, :product_id, :quantity, :status)");
    foreach (const OrderProduct &op, orderProducts) {
        insertQuery.bindValue(":order_id", orderId);
        insertQuery.bindValue(":product_id", op.productId);
        insertQuery.bindValue(":quantity", op.quantity);
        insertQuery.bindValue(":status", op.status);
        if (!insertQuery.exec()) {
            QMessageBox::warning(this, "警告", "无法保存订单成品：" + insertQuery.lastError().text());
        }
    }

    // 根据库存情况调整订单和成品状态
    QString overallStatus = "预下单";
    foreach (const OrderProduct &op, orderProducts) {
        if (op.status == "缺料") {
            overallStatus = "缺料";
            break;
        }
    }
    ui->statusComboBox->setCurrentText(overallStatus);

    // 添加订单操作日志
    QSqlQuery logQuery;
    logQuery.prepare("INSERT INTO OrderLog (order_id, operation_type, operator, operation_date, details) "
                     "VALUES (:order_id, :operation_type, :operator, NOW(), :details)");
    logQuery.bindValue(":order_id", orderId);
    logQuery.bindValue(":operation_type", record.value("id").isNull() ? "CREATE" : "UPDATE");
    logQuery.bindValue(":operator", currentUser);
    logQuery.bindValue(":details", "订单保存");
    if (!logQuery.exec()) {
        QMessageBox::warning(this, "警告", "无法保存订单日志：" + logQuery.lastError().text());
    }

    accept();
}

void OrderDialog::on_cancelButton_clicked()
{
    reject();
}

bool OrderDialog::validateInput()
{
    if (ui->orderNumberLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "订单编号不能为空。");
        return false;
    }
    if (ui->customerNameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "客户名称不能为空。");
        return false;
    }
    return true;
}

void OrderDialog::on_addProductButton_clicked()
{
    // 弹出对话框选择成品
    QSqlQuery productQuery("SELECT id, model, description FROM Product");
    QStringList productList;
    QList<int> productIds;
    while (productQuery.next()) {
        int id = productQuery.value("id").toInt();
        QString model = productQuery.value("model").toString();
        QString description = productQuery.value("description").toString();
        QString displayText = QString("%1 - %2").arg(model).arg(description);
        productList << displayText;
        productIds << id;
    }

    bool ok;
    QString selectedProduct = QInputDialog::getItem(this, "选择成品", "成品：", productList, 0, false, &ok);
    if (ok && !selectedProduct.isEmpty()) {
        int index = productList.indexOf(selectedProduct);
        int productId = productIds.at(index);

        // 输入数量
        int quantity = QInputDialog::getInt(this, "输入数量", "数量：", 1, 1, 1000000, 1, &ok);
        if (ok) {
            // 添加到成品列表
            OrderProduct op = {productId, selectedProduct.section(" - ", 0, 0), selectedProduct.section(" - ", 1), quantity, "未检查"};
            orderProducts.append(op);
            updateProductTable();
        }
    }
}

void OrderDialog::on_removeProductButton_clicked()
{
    int currentRow = ui->productTableWidget->currentRow();
    if (currentRow >= 0) {
        orderProducts.removeAt(currentRow);
        updateProductTable();
    } else {
        QMessageBox::warning(this, "警告", "请先选择要删除的成品。");
    }
}

void OrderDialog::updateProductTable()
{
    ui->productTableWidget->setRowCount(0);
    foreach (const OrderProduct &op, orderProducts) {
        int row = ui->productTableWidget->rowCount();
        ui->productTableWidget->insertRow(row);
        ui->productTableWidget->setItem(row, 0, new QTableWidgetItem(op.productModel));
        ui->productTableWidget->setItem(row, 1, new QTableWidgetItem(op.description));
        ui->productTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(op.quantity)));
        ui->productTableWidget->setItem(row, 3, new QTableWidgetItem(op.status));
    }
}

void OrderDialog::on_checkInventoryButton_clicked()
{
    // 检查每个成品的物料库存
    bool hasInsufficientMaterial = false;
    for (int i = 0; i < orderProducts.size(); ++i) {
        OrderProduct &op = orderProducts[i];
        int productId = op.productId;
        int productQuantity = op.quantity;

        // 查询成品对应的 BOM
        QSqlQuery bomQuery;
        bomQuery.prepare("SELECT material_id, quantity FROM BOM_Material WHERE bom_id = :bom_id");
        bomQuery.bindValue(":bom_id", productId);
        if (bomQuery.exec()) {
            QString insufficientMaterials;
            while (bomQuery.next()) {
                int materialId = bomQuery.value("material_id").toInt();
                int materialQuantityRequired = bomQuery.value("quantity").toInt() * productQuantity;

                // 获取物料库存
                QSqlQuery materialQuery;
                materialQuery.prepare("SELECT material_number, description, good_quantity FROM Material WHERE id = :id");
                materialQuery.bindValue(":id", materialId);
                if (materialQuery.exec() && materialQuery.next()) {
                    int goodQuantity = materialQuery.value("good_quantity").toInt();
                    if (goodQuantity < materialQuantityRequired) {
                        QString materialNumber = materialQuery.value("material_number").toString();
                        QString description = materialQuery.value("description").toString();
                        insufficientMaterials += QString("物料号：%1，描述：%2，库存：%3，需要：%4\n")
                                                     .arg(materialNumber).arg(description).arg(goodQuantity).arg(materialQuantityRequired);
                    }
                }
            }

            if (insufficientMaterials.isEmpty()) {
                op.status = "可生产";
            } else {
                op.status = "缺料";
                hasInsufficientMaterial = true;
                QMessageBox::warning(this, "库存不足", QString("成品 %1 缺料：\n%2").arg(op.productModel).arg(insufficientMaterials));
            }
        } else {
            QMessageBox::warning(this, "警告", "无法获取 BOM 信息：" + bomQuery.lastError().text());
        }
    }

    // 更新成品列表显示
    updateProductTable();

    if (!hasInsufficientMaterial) {
        QMessageBox::information(this, "库存充足", "所有成品的物料库存充足，可以生产。");
    }
}
