#include "materialdialog.h"
#include "ui_materialdialog.h"

MaterialDialog::MaterialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialDialog)
{
    ui->setupUi(this);
}

MaterialDialog::~MaterialDialog()
{
    delete ui;
}

QString MaterialDialog::getName() const {
    return ui->nameLineEdit->text();
}

QString MaterialDialog::getType() const {
    return ui->typeLineEdit->text();
}

int MaterialDialog::getQuantity() const {
    return ui->quantitySpinBox->value();
}

double MaterialDialog::getPrice() const {
    return ui->priceDoubleSpinBox->value();
}

void MaterialDialog::setMaterialData(const QString &name, const QString &type, int quantity, double price) {
    ui->nameLineEdit->setText(name);
    ui->typeLineEdit->setText(type);
    ui->quantitySpinBox->setValue(quantity);
    ui->priceDoubleSpinBox->setValue(price);
}
