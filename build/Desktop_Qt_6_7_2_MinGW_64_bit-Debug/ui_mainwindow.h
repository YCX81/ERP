/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTableWidget *materialTable;
    QHBoxLayout *horizontalLayout;
    QPushButton *addMaterialButton;
    QPushButton *editMaterialButton;
    QPushButton *undoButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        materialTable = new QTableWidget(centralwidget);
        materialTable->setObjectName("materialTable");

        verticalLayout->addWidget(materialTable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        addMaterialButton = new QPushButton(centralwidget);
        addMaterialButton->setObjectName("addMaterialButton");

        horizontalLayout->addWidget(addMaterialButton);

        editMaterialButton = new QPushButton(centralwidget);
        editMaterialButton->setObjectName("editMaterialButton");

        horizontalLayout->addWidget(editMaterialButton);

        undoButton = new QPushButton(centralwidget);
        undoButton->setObjectName("undoButton");

        horizontalLayout->addWidget(undoButton);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        addMaterialButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\347\211\251\346\226\231", nullptr));
        editMaterialButton->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\347\211\251\346\226\231", nullptr));
        undoButton->setText(QCoreApplication::translate("MainWindow", "\346\222\244\351\224\200", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
