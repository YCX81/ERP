// LoginDialog.cpp

#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QCryptographicHash>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    userRole("user") // 默认角色为 'user'
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::getUsername() const
{
    return ui->usernameLineEdit->text();
}

QString LoginDialog::getUserRole() const
{
    return userRole;
}

void LoginDialog::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "用户名或密码不能为空。");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT password_hash, role FROM Users WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        QString storedHash = query.value(0).toString();
        userRole = query.value(1).toString(); // 获取用户角色

        if (storedHash == hashPassword(password)) {
            accept();
        } else {
            QMessageBox::warning(this, "登录失败", "密码错误。");
        }
    } else {
        QMessageBox::warning(this, "登录失败", "用户不存在。");
    }
}

QString LoginDialog::hashPassword(const QString &password) const
{
    // 使用 SHA-256 生成密码哈希值
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}
