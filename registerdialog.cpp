// RegisterDialog.cpp

#include "RegisterDialog.h"
#include "ui_RegisterDialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QDebug>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_registerButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (username.length() < 4 || !passwordRequirementsMet(password)) {
        QMessageBox::warning(this, "注册失败", "用户名需至少4个字符，密码需包含至少6个字符。");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致。");
        return;
    }

    // 检查用户名是否已存在
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Users WHERE username = :username");
    checkQuery.bindValue(":username", username);
    checkQuery.exec();

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "注册失败", "用户名已存在，请选择另一个用户名。");
        return;
    }

    // 插入新用户，默认角色为 'user'
    QSqlQuery query;
    query.prepare("INSERT INTO Users (username, password_hash, role) VALUES (:username, :password_hash, 'user')");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", hashPassword(password));

    if (!query.exec()) {
        qDebug() << "SQL 错误: " << query.lastError().text();
        QMessageBox::warning(this, "注册失败", "无法创建用户，请重试。");
        return;
    }

    accept();
}

QString RegisterDialog::hashPassword(const QString &password) const
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

bool RegisterDialog::passwordRequirementsMet(const QString &password) const
{
    // 只要求最少6个字符
    QRegularExpression re("^.{6,}$");
    QRegularExpressionMatch match = re.match(password);
    return match.hasMatch();
}
