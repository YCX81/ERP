#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QList>

namespace Ui {
class OrderDialog;
}

struct OrderProduct {
    int productId;
    QString productModel;
    QString description;
    int quantity;
    QString status;
};

class OrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderDialog(const QString &currentUser, QWidget *parent = nullptr);
    ~OrderDialog();

    void setOrderData(const QSqlRecord &record);

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    void on_addProductButton_clicked();
    void on_removeProductButton_clicked();
    void on_checkInventoryButton_clicked();

private:
    Ui::OrderDialog *ui;
    QString currentUser;
    QSqlRecord orderRecord;
    QList<OrderProduct> orderProducts;

    QSqlRecord getOrderData();
    bool validateInput();
    void updateProductTable();
};

#endif // ORDERDIALOG_H
