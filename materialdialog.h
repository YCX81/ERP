#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>

namespace Ui {
class MaterialDialog;
}

class MaterialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialDialog(QWidget *parent = nullptr);
    ~MaterialDialog();

    QString getName() const;
    QString getType() const;
    int getQuantity() const;
    double getPrice() const;

    void setMaterialData(const QString &name, const QString &type, int quantity, double price);

private:
    Ui::MaterialDialog *ui;
};

#endif // MATERIALDIALOG_H
