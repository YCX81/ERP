/********************************************************************************
** Form generated from reading UI file 'materialdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALDIALOG_H
#define UI_MATERIALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MaterialDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelName;
    QLineEdit *nameLineEdit;
    QLabel *labelType;
    QLineEdit *typeLineEdit;
    QLabel *labelQuantity;
    QSpinBox *quantitySpinBox;
    QLabel *labelPrice;
    QDoubleSpinBox *priceDoubleSpinBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonBox;
    QPushButton *cancelButton;

    void setupUi(QDialog *MaterialDialog)
    {
        if (MaterialDialog->objectName().isEmpty())
            MaterialDialog->setObjectName("MaterialDialog");
        MaterialDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(MaterialDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelName = new QLabel(MaterialDialog);
        labelName->setObjectName("labelName");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelName);

        nameLineEdit = new QLineEdit(MaterialDialog);
        nameLineEdit->setObjectName("nameLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        labelType = new QLabel(MaterialDialog);
        labelType->setObjectName("labelType");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelType);

        typeLineEdit = new QLineEdit(MaterialDialog);
        typeLineEdit->setObjectName("typeLineEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, typeLineEdit);

        labelQuantity = new QLabel(MaterialDialog);
        labelQuantity->setObjectName("labelQuantity");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelQuantity);

        quantitySpinBox = new QSpinBox(MaterialDialog);
        quantitySpinBox->setObjectName("quantitySpinBox");
        quantitySpinBox->setMinimum(0);

        formLayout->setWidget(2, QFormLayout::FieldRole, quantitySpinBox);

        labelPrice = new QLabel(MaterialDialog);
        labelPrice->setObjectName("labelPrice");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelPrice);

        priceDoubleSpinBox = new QDoubleSpinBox(MaterialDialog);
        priceDoubleSpinBox->setObjectName("priceDoubleSpinBox");
        priceDoubleSpinBox->setMinimum(0.000000000000000);
        priceDoubleSpinBox->setDecimals(2);

        formLayout->setWidget(3, QFormLayout::FieldRole, priceDoubleSpinBox);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        buttonBox = new QPushButton(MaterialDialog);
        buttonBox->setObjectName("buttonBox");

        horizontalLayout->addWidget(buttonBox);

        cancelButton = new QPushButton(MaterialDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(MaterialDialog);

        QMetaObject::connectSlotsByName(MaterialDialog);
    } // setupUi

    void retranslateUi(QDialog *MaterialDialog)
    {
        MaterialDialog->setWindowTitle(QCoreApplication::translate("MaterialDialog", "\347\211\251\346\226\231\344\277\241\346\201\257", nullptr));
        labelName->setText(QCoreApplication::translate("MaterialDialog", "\345\220\215\347\247\260:", nullptr));
        labelType->setText(QCoreApplication::translate("MaterialDialog", "\347\261\273\345\236\213:", nullptr));
        labelQuantity->setText(QCoreApplication::translate("MaterialDialog", "\346\225\260\351\207\217:", nullptr));
        labelPrice->setText(QCoreApplication::translate("MaterialDialog", "\344\273\267\346\240\274:", nullptr));
        buttonBox->setText(QCoreApplication::translate("MaterialDialog", "\347\241\256\345\256\232", nullptr));
        cancelButton->setText(QCoreApplication::translate("MaterialDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MaterialDialog: public Ui_MaterialDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALDIALOG_H
