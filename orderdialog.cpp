#include "orderdialog.h"
#include "ui_orderdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QDebug>
#include <QUuid> // 用于生成唯一的 order_number
#include <QSqlField>

OrderDialog::OrderDialog(const QString &currentUser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderDialog),
    currentUser(currentUser)
{
    ui->setupUi(this);

    // 初始化状态下拉框
    ui->statusComboBox->addItems(QStringList() << "预下单" << "缺料" << "制作中" << "制作完毕" << "已出库");

    // 初始化成品列表
    ui->productTableWidget->setColumnCount(5); // 更新为5列
    QStringList headers;
    headers << "成品型号" << "描述" << "BOM名称" << "数量" << "状态";
    ui->productTableWidget->setHorizontalHeaderLabels(headers);
    ui->productTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->productTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置维护人
    ui->maintainerLineEdit->setText(currentUser);
    ui->maintainerLineEdit->setReadOnly(true);
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
    ui->maintainerLineEdit->setText(record.value("maintainer").toString());

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

                OrderProduct op;
                op.productId = productId;
                op.productModel = model;
                op.description = description;
                op.quantity = quantity;
                op.status = status;
                orderProducts.append(op);
            }
        }
        updateProductTable();
    }
}

QSqlRecord OrderDialog::getOrderData()
{
    QSqlRecord record = orderRecord;

    QString orderNumber = ui->orderNumberLineEdit->text().trimmed();

    // 添加调试信息
    qDebug() << "getOrderData() - Order Number read from UI:" << orderNumber;

    // 如果 record 为空（新订单），添加必要的字段
    if (record.isEmpty() || record.count() == 0) {
        record.append(QSqlField("order_number", QVariant::String));
        record.append(QSqlField("customer_name", QVariant::String));
        record.append(QSqlField("status", QVariant::String));
        record.append(QSqlField("remarks", QVariant::String));
        record.append(QSqlField("maintainer", QVariant::String));
    }

    record.setValue("order_number", orderNumber);
    record.setValue("customer_name", ui->customerNameLineEdit->text().trimmed());
    record.setValue("status", ui->statusComboBox->currentText());
    record.setValue("remarks", ui->remarksTextEdit->toPlainText().trimmed());
    record.setValue("maintainer", ui->maintainerLineEdit->text().trimmed());

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

    QString orderNumber = record.value("order_number").toString();
    QString customerName = record.value("customer_name").toString();
    QString status = record.value("status").toString();
    QString remarks = record.value("remarks").toString();
    QString maintainer = record.value("maintainer").toString();

    // 添加调试信息
    qDebug() << "on_saveButton_clicked() - Saving Order - Order Number:" << orderNumber;
    QMessageBox::information(this, "调试", QString("Order Number: %1").arg(orderNumber));

    QSqlQuery query;
    bool isNew = orderRecord.value("id").isNull() || orderRecord.value("id").toInt() == 0;

    // 开始事务
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::critical(this, "错误", "无法启动事务：" + db.lastError().text());
        return;
    }

    int orderId;
    if (isNew) {
        // 新增订单
        query.prepare("INSERT INTO `Order` (order_number, customer_name, status, remarks, maintainer, created_at, updated_at) "
                      "VALUES (:order_number, :customer_name, :status, :remarks, :maintainer, NOW(), NOW())");
        query.bindValue(":order_number", orderNumber);
        query.bindValue(":customer_name", customerName);
        query.bindValue(":status", status);
        query.bindValue(":remarks", remarks);
        query.bindValue(":maintainer", maintainer);
        qDebug() << "Executing INSERT Order with order_number:" << orderNumber;
        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(this, "错误", "无法保存订单信息：" + query.lastError().text());
            qDebug() << "Failed to insert Order:" << query.lastError().text();
            return;
        }
        orderId = query.lastInsertId().toInt();
        qDebug() << "Inserted Order with ID:" << orderId;
    } else {
        // 更新订单
        query.prepare("UPDATE `Order` SET order_number = :order_number, customer_name = :customer_name, "
                      "status = :status, remarks = :remarks, maintainer = :maintainer, updated_at = NOW() WHERE id = :id");
        query.bindValue(":order_number", orderNumber);
        query.bindValue(":customer_name", customerName);
        query.bindValue(":status", status);
        query.bindValue(":remarks", remarks);
        query.bindValue(":maintainer", maintainer);
        query.bindValue(":id", orderRecord.value("id").toInt());
        qDebug() << "Executing UPDATE Order with id:" << orderRecord.value("id").toInt();
        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(this, "错误", "无法更新订单信息：" + query.lastError().text());
            qDebug() << "Failed to update Order:" << query.lastError().text();
            return;
        }
        orderId = orderRecord.value("id").toInt();
        qDebug() << "Updated Order with ID:" << orderId;
    }

    // 删除旧的订单成品
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM Order_Product WHERE order_id = :order_id");
    deleteQuery.bindValue(":order_id", orderId);
    qDebug() << "Executing DELETE Order_Product for Order ID:" << orderId;
    if (!deleteQuery.exec()) {
        db.rollback();
        QMessageBox::warning(this, "警告", "无法删除旧的订单成品：" + deleteQuery.lastError().text());
        qDebug() << "Failed to delete old Order_Product:" << deleteQuery.lastError().text();
        // 继续执行，因为可能已经删除了一部分物料
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
        qDebug() << "Inserting Order_Product - Order ID:" << orderId << "Product ID:" << op.productId << "Quantity:" << op.quantity;
        if (!insertQuery.exec()) {
            db.rollback();
            QMessageBox::warning(this, "警告", "无法保存订单成品：" + insertQuery.lastError().text());
            qDebug() << "Failed to insert Order_Product:" << insertQuery.lastError().text();
            return;
        }
    }

    // 添加订单操作日志
    QSqlQuery logQuery;
    logQuery.prepare("INSERT INTO OrderLog (order_id, operation_type, operator, operation_date, details) "
                     "VALUES (:order_id, :operation_type, :operator, NOW(), :details)");
    logQuery.bindValue(":order_id", orderId);
    logQuery.bindValue(":operation_type", isNew ? "CREATE" : "UPDATE");
    logQuery.bindValue(":operator", currentUser);
    logQuery.bindValue(":details", "订单保存");
    if (!logQuery.exec()) {
        QMessageBox::warning(this, "警告", "无法保存订单日志：" + logQuery.lastError().text());
        qDebug() << "Failed to insert OrderLog:" << logQuery.lastError().text();
    }

    // 提交事务
    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "错误", "无法提交事务：" + db.lastError().text());
        qDebug() << "Failed to commit transaction:" << db.lastError().text();
        return;
    }

    qDebug() << "Order saved successfully with ID:" << orderId;
    accept();
}

void OrderDialog::on_cancelButton_clicked()
{
    reject();
}

bool OrderDialog::validateInput()
{
    QString orderNumber = ui->orderNumberLineEdit->text().trimmed();
    QString customerName = ui->customerNameLineEdit->text().trimmed();

    qDebug() << "Validating Input - Order Number:" << orderNumber << ", Customer Name:" << customerName;

    if (orderNumber.isEmpty()) {
        QMessageBox::warning(this, "警告", "订单编号不能为空。");
        return false;
    }
    if (customerName.isEmpty()) {
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
        if (index < 0 || index >= productIds.size()) {
            QMessageBox::warning(this, "警告", "选择的成品无效。");
            return;
        }
        int productId = productIds.at(index);

        // 检查产品是否关联 BOM
        QSqlQuery bomCheckQuery;
        bomCheckQuery.prepare("SELECT bom_id FROM Product WHERE id = :product_id");
        bomCheckQuery.bindValue(":product_id", productId);
        if (bomCheckQuery.exec() && bomCheckQuery.next()) {
            int bomId = bomCheckQuery.value("bom_id").toInt();
            if (bomId == 0) {
                QMessageBox::warning(this, "警告", QString("所选产品 %1 未关联任何 BOM。请先关联 BOM。").arg(selectedProduct.section(" - ", 0, 0)));
                return;
            }
        } else {
            QMessageBox::warning(this, "警告", "无法验证产品的 BOM 关联：" + bomCheckQuery.lastError().text());
            return;
        }

        // 输入数量
        int quantity = QInputDialog::getInt(this, "输入数量", "数量：", 1, 1, 1000000, 1, &ok);
        if (ok) {
            // 添加到成品列表
            OrderProduct op;
            op.productId = productId;
            op.productModel = selectedProduct.section(" - ", 0, 0);
            op.description = selectedProduct.section(" - ", 1);
            op.quantity = quantity;
            op.status = "未检查";
            orderProducts.append(op);
            updateProductTable();
        }
    }
}

void OrderDialog::on_removeProductButton_clicked()
{
    int currentRow = ui->productTableWidget->currentRow();
    if (currentRow >= 0 && currentRow < orderProducts.size()) {
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
        // 获取 BOM 名称
        QString bomName = "无";
        QSqlQuery bomQuery;
        bomQuery.prepare("SELECT name FROM BOM WHERE id = (SELECT bom_id FROM Product WHERE id = :product_id)");
        bomQuery.bindValue(":product_id", op.productId);
        if (bomQuery.exec() && bomQuery.next()) {
            bomName = bomQuery.value("name").toString();
        }

        int row = ui->productTableWidget->rowCount();
        ui->productTableWidget->insertRow(row);
        ui->productTableWidget->setItem(row, 0, new QTableWidgetItem(op.productModel));
        ui->productTableWidget->setItem(row, 1, new QTableWidgetItem(op.description));
        ui->productTableWidget->setItem(row, 2, new QTableWidgetItem(bomName));
        ui->productTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(op.quantity)));
        ui->productTableWidget->setItem(row, 4, new QTableWidgetItem(op.status));
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

        // 查询产品对应的 BOM
        QSqlQuery bomQuery;
        bomQuery.prepare("SELECT bom_id FROM Product WHERE id = :product_id");
        bomQuery.bindValue(":product_id", productId);
        if (bomQuery.exec() && bomQuery.next()) {
            int bomId = bomQuery.value("bom_id").toInt();
            if (bomId == 0) {
                QMessageBox::warning(this, "警告", QString("产品 %1 没有关联的 BOM。").arg(op.productModel));
                continue;
            }

            // 查询 BOM 的物料清单
            QSqlQuery materialsQuery;
            materialsQuery.prepare("SELECT m.material_number, m.description, m.good_quantity, bm.quantity "
                                   "FROM BOM_Material bm "
                                   "JOIN Material m ON bm.material_id = m.id "
                                   "WHERE bm.bom_id = :bom_id");
            materialsQuery.bindValue(":bom_id", bomId);
            if (materialsQuery.exec()) {
                QString insufficientMaterials;
                while (materialsQuery.next()) {
                    QString materialNumber = materialsQuery.value("material_number").toString();
                    QString description = materialsQuery.value("description").toString();
                    int bomQuantityPerProduct = materialsQuery.value("quantity").toInt();
                    int totalRequired = bomQuantityPerProduct * productQuantity;
                    int stock = materialsQuery.value("good_quantity").toInt();

                    if (stock < totalRequired) {
                        insufficientMaterials += QString("物料号：%1，描述：%2，库存：%3，需要：%4\n")
                                                     .arg(materialNumber)
                                                     .arg(description)
                                                     .arg(stock)
                                                     .arg(totalRequired);
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
                QMessageBox::warning(this, "警告", "无法获取 BOM 的物料清单：" + materialsQuery.lastError().text());
            }
        } else {
            QMessageBox::warning(this, "警告", "无法获取产品的 BOM 信息：" + bomQuery.lastError().text());
        }
    }

    // 更新成品列表显示
    updateProductTable();

    // 根据成品状态调整订单状态
    QString overallStatus = "预下单";
    for (const OrderProduct &op : orderProducts) {
        if (op.status == "缺料") {
            overallStatus = "缺料";
            break;
        }
    }
    ui->statusComboBox->setCurrentText(overallStatus);

    // 更新订单状态到数据库
    int orderId = orderRecord.value("id").toInt(); // 确保获取正确的 orderId
    QSqlQuery updateStatusQuery;
    updateStatusQuery.prepare("UPDATE `Order` SET status = :status WHERE id = :id");
    updateStatusQuery.bindValue(":status", overallStatus);
    updateStatusQuery.bindValue(":id", orderId);
    if (!updateStatusQuery.exec()) {
        QMessageBox::warning(this, "警告", "无法更新订单状态：" + updateStatusQuery.lastError().text());
        qDebug() << "Failed to update Order status:" << updateStatusQuery.lastError().text();
    }

    if (!hasInsufficientMaterial) {
        QMessageBox::information(this, "库存充足", "所有成品的物料库存充足，可以生产。");
    }
}
