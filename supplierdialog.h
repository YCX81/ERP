#ifndef SUPPLIERDIALOG_H
#define SUPPLIERDIALOG_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class SupplierDialog;
}

struct SupplierInfo {
    QString supplier_id;
    QString supplier_name;
    QString business_nature;
    QString primary_contact;
    QString primary_contact_phone;
    QString secondary_contact;
    QString secondary_contact_phone;
    QString address;
    QString supplier_manager;
};

Q_DECLARE_METATYPE(SupplierInfo)

class SupplierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SupplierDialog(const QString &currentUser, QWidget *parent = nullptr);
    ~SupplierDialog();

    void setSupplierData(const QSqlRecord &record);
    QSqlRecord getSupplierData() const;

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::SupplierDialog *ui;
    QSqlRecord supplierRecord;
    QString currentUser;

    bool validateInput();
};

#endif // SUPPLIERDIALOG_H
