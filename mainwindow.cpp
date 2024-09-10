#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "user.h"
#include "material.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QCryptographicHash>
#include <QSqlError>
#include <QCheckBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QSettings>

QSqlTableModel *materialModel = nullptr;  // 在这里定义 materialModel

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginDialog(nullptr)
    , usernameEdit(nullptr)
    , passwordEdit(nullptr)
    , rememberMeCheckBox(nullptr)
{
    ui->setupUi(this);

    if (connectToDatabase()) {
        materialModel = new QSqlTableModel(this, db); // 初始化 materialModel
        materialModel->setTable("materials");
        materialModel->select();
        ui->tableViewMaterials->setModel(materialModel);

        connect(ui->buttonLoginMain, &QPushButton::clicked, this, &MainWindow::on_buttonLoginMain_clicked);
        connect(ui->buttonAddMaterial, &QPushButton::clicked, this, &MainWindow::handleAddMaterial);
        connect(ui->buttonRemoveMaterial, &QPushButton::clicked, this, &MainWindow::handleRemoveMaterial);
        connect(ui->buttonEditMaterial, &QPushButton::clicked, this, &MainWindow::handleEditMaterial);

        loadSettings();
    }
}

// 其余代码保持不变


MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::on_buttonLoginMain_clicked()
{
    loginDialog = new QDialog(this);
    loginDialog->setWindowTitle("登录");

    QVBoxLayout *layout = new QVBoxLayout(loginDialog);

    QLabel *usernameLabel = new QLabel("用户名:", loginDialog);
    usernameEdit = new QLineEdit(loginDialog);
    QLabel *passwordLabel = new QLabel("密码:", loginDialog);
    passwordEdit = new QLineEdit(loginDialog);
    passwordEdit->setEchoMode(QLineEdit::Password);

    rememberMeCheckBox = new QCheckBox("记住我", loginDialog);

    QPushButton *loginButton = new QPushButton("登录", loginDialog);
    QPushButton *registerButton = new QPushButton("注册", loginDialog);

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLoginDialogAccepted);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::handleRegisterUser);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(rememberMeCheckBox);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);

    loginDialog->setLayout(layout);
    loginDialog->exec();
}

void MainWindow::handleLoginDialogAccepted()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (authenticateUser(username, password)) {
        QMessageBox::information(this, "登录成功", "欢迎回来，" + username + "!");
        loginDialog->accept();
        if (rememberMeCheckBox->isChecked()) {
            QSettings settings("MyCompany", "ERPSystem");
            settings.setValue("username", username);
            settings.setValue("rememberMe", true);
        } else {
            QSettings settings("MyCompany", "ERPSystem");
            settings.setValue("rememberMe", false);
        }
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误，请重试。");
    }
}

void MainWindow::handleAddMaterial()
{
    addMaterial(); // 调用Material模块中的函数
}

void MainWindow::handleRemoveMaterial()
{
    removeMaterial(); // 调用Material模块中的函数
}

void MainWindow::handleEditMaterial()
{
    editMaterial(); // 调用Material模块中的函数
}

void MainWindow::handleRegisterUser()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (registerUser(username, password)) {
        QMessageBox::information(this, "注册成功", "用户注册成功！");
    } else {
        QMessageBox::warning(this, "注册失败", "用户注册失败，可能是用户名已存在。");
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("MyCompany", "ERPSystem");
    if (settings.value("rememberMe", false).toBool()) {
        if (usernameEdit) {
            usernameEdit->setText(settings.value("username", "").toString());
        }
        if (rememberMeCheckBox) {
            rememberMeCheckBox->setChecked(true);
        }
    } else {
        if (rememberMeCheckBox) {
            rememberMeCheckBox->setChecked(false);
        }
    }
}

void MainWindow::saveSettings()
{
    QSettings settings("MyCompany", "ERPSystem");
    if (rememberMeCheckBox && rememberMeCheckBox->isChecked()) {
        settings.setValue("username", usernameEdit->text());
        settings.setValue("rememberMe", true);
    } else {
        settings.setValue("rememberMe", false);
    }
}
