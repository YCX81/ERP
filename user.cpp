#include "user.h"
#include "database.h"
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

bool authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec()) {
        if (query.next()) {
            QString storedPassword = query.value(0).toString();
            QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
            return storedPassword == hashedPassword;
        }
    } else {
        QMessageBox::warning(nullptr, "登录错误", query.lastError().text());
    }

    return false;
}

bool registerUser(const QString &username, const QString &password)
{
    QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "错误", "注册失败：" + query.lastError().text());
        return false;
    }

    return true;
}
