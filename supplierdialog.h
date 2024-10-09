#ifndef SUPPLIERDIALOG_H
#define SUPPLIERDIALOG_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class SupplierDialog;
}

class SupplierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SupplierDialog(QWidget *parent = nullptr);
    ~SupplierDialog();

    void setSupplierData(const QSqlRecord &record);
    QSqlRecord getSupplierData() const;

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::SupplierDialog *ui;
    QSqlRecord supplierRecord;

    bool validateInput();
};

#endif // SUPPLIERDIALOG_H
