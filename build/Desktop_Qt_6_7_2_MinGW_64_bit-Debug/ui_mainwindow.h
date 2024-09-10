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
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QWidget *centralwidget;
    QVBoxLayout *mainVerticalLayout;
    QToolButton *buttonLoginMain;
    QHBoxLayout *topHorizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabMaterials;
    QVBoxLayout *verticalLayoutMaterials;
    QSpacerItem *horizontalSpacer;
    QTableView *tableViewMaterials;
    QGridLayout *gridLayoutMaterialDetails;
    QLabel *labelCategory;
    QComboBox *comboBoxCategory;
    QLabel *labelPurchaseType;
    QComboBox *comboBoxPurchaseType;
    QLabel *labelDescription;
    QLineEdit *lineEditDescription;
    QLabel *labelVersion;
    QLineEdit *lineEditVersion;
    QLabel *labelSN;
    QLineEdit *lineEditSN;
    QLabel *labelStatus;
    QComboBox *comboBoxStatus;
    QLabel *labelSupplier;
    QComboBox *comboBoxSupplier;
    QLabel *labelSupplierPartNumber;
    QLineEdit *lineEditSupplierPartNumber;
    QLabel *labelSupplierSpec;
    QPushButton *buttonUploadSupplierSpec;
    QLabel *labelDeliveryTime;
    QDateEdit *dateEditDeliveryTime;
    QLabel *labelPrice;
    QHBoxLayout *horizontalLayoutPrice;
    QComboBox *comboBoxCurrency;
    QDoubleSpinBox *doubleSpinBoxPrice;
    QLabel *labelQuantity;
    QSpinBox *spinBoxQuantity;
    QLabel *labelRemarks;
    QLineEdit *lineEditRemarks;
    QLabel *labelPhoto;
    QPushButton *buttonUploadPhoto;
    QLabel *labelLocation;
    QLineEdit *lineEditLocation;
    QHBoxLayout *horizontalLayoutMaterials;
    QPushButton *buttonAddMaterial;
    QPushButton *buttonRemoveMaterial;
    QPushButton *buttonEditMaterial;
    QPushButton *buttonExport;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 768);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName("actionUndo");
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName("actionRedo");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainVerticalLayout = new QVBoxLayout(centralwidget);
        mainVerticalLayout->setObjectName("mainVerticalLayout");
        buttonLoginMain = new QToolButton(centralwidget);
        buttonLoginMain->setObjectName("buttonLoginMain");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/login.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        buttonLoginMain->setIcon(icon);

        mainVerticalLayout->addWidget(buttonLoginMain);

        topHorizontalLayout = new QHBoxLayout();
        topHorizontalLayout->setObjectName("topHorizontalLayout");

        mainVerticalLayout->addLayout(topHorizontalLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabMaterials = new QWidget();
        tabMaterials->setObjectName("tabMaterials");
        verticalLayoutMaterials = new QVBoxLayout(tabMaterials);
        verticalLayoutMaterials->setObjectName("verticalLayoutMaterials");
        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutMaterials->addItem(horizontalSpacer);

        tableViewMaterials = new QTableView(tabMaterials);
        tableViewMaterials->setObjectName("tableViewMaterials");

        verticalLayoutMaterials->addWidget(tableViewMaterials);

        gridLayoutMaterialDetails = new QGridLayout();
        gridLayoutMaterialDetails->setObjectName("gridLayoutMaterialDetails");
        labelCategory = new QLabel(tabMaterials);
        labelCategory->setObjectName("labelCategory");

        gridLayoutMaterialDetails->addWidget(labelCategory, 0, 0, 1, 1);

        comboBoxCategory = new QComboBox(tabMaterials);
        comboBoxCategory->addItem(QString());
        comboBoxCategory->addItem(QString());
        comboBoxCategory->setObjectName("comboBoxCategory");

        gridLayoutMaterialDetails->addWidget(comboBoxCategory, 0, 1, 1, 1);

        labelPurchaseType = new QLabel(tabMaterials);
        labelPurchaseType->setObjectName("labelPurchaseType");

        gridLayoutMaterialDetails->addWidget(labelPurchaseType, 1, 0, 1, 1);

        comboBoxPurchaseType = new QComboBox(tabMaterials);
        comboBoxPurchaseType->addItem(QString());
        comboBoxPurchaseType->addItem(QString());
        comboBoxPurchaseType->setObjectName("comboBoxPurchaseType");

        gridLayoutMaterialDetails->addWidget(comboBoxPurchaseType, 1, 1, 1, 1);

        labelDescription = new QLabel(tabMaterials);
        labelDescription->setObjectName("labelDescription");

        gridLayoutMaterialDetails->addWidget(labelDescription, 2, 0, 1, 1);

        lineEditDescription = new QLineEdit(tabMaterials);
        lineEditDescription->setObjectName("lineEditDescription");

        gridLayoutMaterialDetails->addWidget(lineEditDescription, 2, 1, 1, 1);

        labelVersion = new QLabel(tabMaterials);
        labelVersion->setObjectName("labelVersion");

        gridLayoutMaterialDetails->addWidget(labelVersion, 3, 0, 1, 1);

        lineEditVersion = new QLineEdit(tabMaterials);
        lineEditVersion->setObjectName("lineEditVersion");

        gridLayoutMaterialDetails->addWidget(lineEditVersion, 3, 1, 1, 1);

        labelSN = new QLabel(tabMaterials);
        labelSN->setObjectName("labelSN");

        gridLayoutMaterialDetails->addWidget(labelSN, 4, 0, 1, 1);

        lineEditSN = new QLineEdit(tabMaterials);
        lineEditSN->setObjectName("lineEditSN");

        gridLayoutMaterialDetails->addWidget(lineEditSN, 4, 1, 1, 1);

        labelStatus = new QLabel(tabMaterials);
        labelStatus->setObjectName("labelStatus");

        gridLayoutMaterialDetails->addWidget(labelStatus, 5, 0, 1, 1);

        comboBoxStatus = new QComboBox(tabMaterials);
        comboBoxStatus->addItem(QString());
        comboBoxStatus->addItem(QString());
        comboBoxStatus->setObjectName("comboBoxStatus");

        gridLayoutMaterialDetails->addWidget(comboBoxStatus, 5, 1, 1, 1);

        labelSupplier = new QLabel(tabMaterials);
        labelSupplier->setObjectName("labelSupplier");

        gridLayoutMaterialDetails->addWidget(labelSupplier, 6, 0, 1, 1);

        comboBoxSupplier = new QComboBox(tabMaterials);
        comboBoxSupplier->addItem(QString());
        comboBoxSupplier->addItem(QString());
        comboBoxSupplier->setObjectName("comboBoxSupplier");

        gridLayoutMaterialDetails->addWidget(comboBoxSupplier, 6, 1, 1, 1);

        labelSupplierPartNumber = new QLabel(tabMaterials);
        labelSupplierPartNumber->setObjectName("labelSupplierPartNumber");

        gridLayoutMaterialDetails->addWidget(labelSupplierPartNumber, 7, 0, 1, 1);

        lineEditSupplierPartNumber = new QLineEdit(tabMaterials);
        lineEditSupplierPartNumber->setObjectName("lineEditSupplierPartNumber");

        gridLayoutMaterialDetails->addWidget(lineEditSupplierPartNumber, 7, 1, 1, 1);

        labelSupplierSpec = new QLabel(tabMaterials);
        labelSupplierSpec->setObjectName("labelSupplierSpec");

        gridLayoutMaterialDetails->addWidget(labelSupplierSpec, 8, 0, 1, 1);

        buttonUploadSupplierSpec = new QPushButton(tabMaterials);
        buttonUploadSupplierSpec->setObjectName("buttonUploadSupplierSpec");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/upload.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        buttonUploadSupplierSpec->setIcon(icon1);

        gridLayoutMaterialDetails->addWidget(buttonUploadSupplierSpec, 8, 1, 1, 1);

        labelDeliveryTime = new QLabel(tabMaterials);
        labelDeliveryTime->setObjectName("labelDeliveryTime");

        gridLayoutMaterialDetails->addWidget(labelDeliveryTime, 9, 0, 1, 1);

        dateEditDeliveryTime = new QDateEdit(tabMaterials);
        dateEditDeliveryTime->setObjectName("dateEditDeliveryTime");

        gridLayoutMaterialDetails->addWidget(dateEditDeliveryTime, 9, 1, 1, 1);

        labelPrice = new QLabel(tabMaterials);
        labelPrice->setObjectName("labelPrice");

        gridLayoutMaterialDetails->addWidget(labelPrice, 10, 0, 1, 1);

        horizontalLayoutPrice = new QHBoxLayout();
        horizontalLayoutPrice->setObjectName("horizontalLayoutPrice");
        comboBoxCurrency = new QComboBox(tabMaterials);
        comboBoxCurrency->addItem(QString());
        comboBoxCurrency->addItem(QString());
        comboBoxCurrency->addItem(QString());
        comboBoxCurrency->setObjectName("comboBoxCurrency");

        horizontalLayoutPrice->addWidget(comboBoxCurrency);

        doubleSpinBoxPrice = new QDoubleSpinBox(tabMaterials);
        doubleSpinBoxPrice->setObjectName("doubleSpinBoxPrice");

        horizontalLayoutPrice->addWidget(doubleSpinBoxPrice);


        gridLayoutMaterialDetails->addLayout(horizontalLayoutPrice, 10, 1, 1, 1);

        labelQuantity = new QLabel(tabMaterials);
        labelQuantity->setObjectName("labelQuantity");

        gridLayoutMaterialDetails->addWidget(labelQuantity, 11, 0, 1, 1);

        spinBoxQuantity = new QSpinBox(tabMaterials);
        spinBoxQuantity->setObjectName("spinBoxQuantity");

        gridLayoutMaterialDetails->addWidget(spinBoxQuantity, 11, 1, 1, 1);

        labelRemarks = new QLabel(tabMaterials);
        labelRemarks->setObjectName("labelRemarks");

        gridLayoutMaterialDetails->addWidget(labelRemarks, 12, 0, 1, 1);

        lineEditRemarks = new QLineEdit(tabMaterials);
        lineEditRemarks->setObjectName("lineEditRemarks");

        gridLayoutMaterialDetails->addWidget(lineEditRemarks, 12, 1, 1, 1);

        labelPhoto = new QLabel(tabMaterials);
        labelPhoto->setObjectName("labelPhoto");

        gridLayoutMaterialDetails->addWidget(labelPhoto, 13, 0, 1, 1);

        buttonUploadPhoto = new QPushButton(tabMaterials);
        buttonUploadPhoto->setObjectName("buttonUploadPhoto");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/photo.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        buttonUploadPhoto->setIcon(icon2);

        gridLayoutMaterialDetails->addWidget(buttonUploadPhoto, 13, 1, 1, 1);

        labelLocation = new QLabel(tabMaterials);
        labelLocation->setObjectName("labelLocation");

        gridLayoutMaterialDetails->addWidget(labelLocation, 14, 0, 1, 1);

        lineEditLocation = new QLineEdit(tabMaterials);
        lineEditLocation->setObjectName("lineEditLocation");

        gridLayoutMaterialDetails->addWidget(lineEditLocation, 14, 1, 1, 1);


        verticalLayoutMaterials->addLayout(gridLayoutMaterialDetails);

        horizontalLayoutMaterials = new QHBoxLayout();
        horizontalLayoutMaterials->setObjectName("horizontalLayoutMaterials");
        buttonAddMaterial = new QPushButton(tabMaterials);
        buttonAddMaterial->setObjectName("buttonAddMaterial");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        buttonAddMaterial->setIcon(icon3);

        horizontalLayoutMaterials->addWidget(buttonAddMaterial);

        buttonRemoveMaterial = new QPushButton(tabMaterials);
        buttonRemoveMaterial->setObjectName("buttonRemoveMaterial");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/delete.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        buttonRemoveMaterial->setIcon(icon4);

        horizontalLayoutMaterials->addWidget(buttonRemoveMaterial);

        buttonEditMaterial = new QPushButton(tabMaterials);
        buttonEditMaterial->setObjectName("buttonEditMaterial");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/edit.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        buttonEditMaterial->setIcon(icon5);

        horizontalLayoutMaterials->addWidget(buttonEditMaterial);

        buttonExport = new QPushButton(tabMaterials);
        buttonExport->setObjectName("buttonExport");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/export.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        buttonExport->setIcon(icon6);

        horizontalLayoutMaterials->addWidget(buttonExport);


        verticalLayoutMaterials->addLayout(horizontalLayoutMaterials);

        tabWidget->addTab(tabMaterials, QString());

        mainVerticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1024, 18));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ERP\347\263\273\347\273\237", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
        actionUndo->setText(QCoreApplication::translate("MainWindow", "\346\222\244\351\224\200", nullptr));
        actionRedo->setText(QCoreApplication::translate("MainWindow", "\351\207\215\345\201\232", nullptr));
        buttonLoginMain->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
        labelCategory->setText(QCoreApplication::translate("MainWindow", "\347\261\273\345\210\253", nullptr));
        comboBoxCategory->setItemText(0, QCoreApplication::translate("MainWindow", "\347\224\265\345\255\220\345\205\203\344\273\266", nullptr));
        comboBoxCategory->setItemText(1, QCoreApplication::translate("MainWindow", "\346\234\272\346\242\260\351\233\266\344\273\266", nullptr));

        labelPurchaseType->setText(QCoreApplication::translate("MainWindow", "\351\207\207\350\264\255\347\261\273\345\236\213", nullptr));
        comboBoxPurchaseType->setItemText(0, QCoreApplication::translate("MainWindow", "\350\207\252\345\210\266", nullptr));
        comboBoxPurchaseType->setItemText(1, QCoreApplication::translate("MainWindow", "\351\207\207\350\264\255", nullptr));

        labelDescription->setText(QCoreApplication::translate("MainWindow", "\346\217\217\350\277\260", nullptr));
        labelVersion->setText(QCoreApplication::translate("MainWindow", "\347\211\210\346\234\254\345\217\267", nullptr));
        labelSN->setText(QCoreApplication::translate("MainWindow", "\345\272\217\345\210\227\345\217\267", nullptr));
        labelStatus->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201", nullptr));
        comboBoxStatus->setItemText(0, QCoreApplication::translate("MainWindow", "PP", nullptr));
        comboBoxStatus->setItemText(1, QCoreApplication::translate("MainWindow", "FP", nullptr));

        labelSupplier->setText(QCoreApplication::translate("MainWindow", "\344\276\233\345\272\224\345\225\206", nullptr));
        comboBoxSupplier->setItemText(0, QCoreApplication::translate("MainWindow", "\344\276\233\345\272\224\345\225\206A", nullptr));
        comboBoxSupplier->setItemText(1, QCoreApplication::translate("MainWindow", "\344\276\233\345\272\224\345\225\206B", nullptr));

        labelSupplierPartNumber->setText(QCoreApplication::translate("MainWindow", "\344\276\233\345\272\224\345\225\206\346\226\231\345\217\267", nullptr));
        labelSupplierSpec->setText(QCoreApplication::translate("MainWindow", "\344\276\233\345\272\224\345\225\206\350\247\204\346\240\274\344\271\246", nullptr));
        buttonUploadSupplierSpec->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240\350\247\204\346\240\274\344\271\246", nullptr));
        labelDeliveryTime->setText(QCoreApplication::translate("MainWindow", "\350\264\247\346\234\237", nullptr));
        labelPrice->setText(QCoreApplication::translate("MainWindow", "\345\215\225\344\273\267", nullptr));
        comboBoxCurrency->setItemText(0, QCoreApplication::translate("MainWindow", "\302\245 \344\272\272\346\260\221\345\270\201", nullptr));
        comboBoxCurrency->setItemText(1, QCoreApplication::translate("MainWindow", "$ \347\276\216\345\205\203", nullptr));
        comboBoxCurrency->setItemText(2, QCoreApplication::translate("MainWindow", "\302\243 \350\213\261\351\225\221", nullptr));

        labelQuantity->setText(QCoreApplication::translate("MainWindow", "\346\225\260\351\207\217", nullptr));
        labelRemarks->setText(QCoreApplication::translate("MainWindow", "\345\244\207\346\263\250", nullptr));
        labelPhoto->setText(QCoreApplication::translate("MainWindow", "\345\256\236\347\211\251\347\205\247\347\211\207", nullptr));
        buttonUploadPhoto->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240\347\205\247\347\211\207", nullptr));
        labelLocation->setText(QCoreApplication::translate("MainWindow", "\345\272\223\344\275\215", nullptr));
        buttonAddMaterial->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\347\211\251\346\226\231", nullptr));
        buttonRemoveMaterial->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\347\211\251\346\226\231", nullptr));
        buttonEditMaterial->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\347\211\251\346\226\231", nullptr));
        buttonExport->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272Excel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMaterials), QCoreApplication::translate("MainWindow", "\347\211\251\346\226\231\347\256\241\347\220\206", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
