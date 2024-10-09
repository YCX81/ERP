#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class OrderDialog;
}

class OrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderDialog(const QString &currentUser, QWidget *parent = nullptr);
    ~OrderDialog();

    void setOrderData(const QSqlRecord &record);
    QSqlRecord getOrderData() const;

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    void on_checkInventoryButton_clicked();
    void on_addProductButton_clicked();
    void on_removeProductButton_clicked();

private:
    Ui::OrderDialog *ui;
    QSqlRecord orderRecord;
    QString currentUser;

    bool validateInput();

    // 存储订单成品列表
    struct OrderProduct {
        int productId;
        QString productModel;
        QString description;
        int quantity;
        QString status;
    };
    QList<OrderProduct> orderProducts;

    void updateProductTable();
};

#endif // ORDERDIALOG_H
