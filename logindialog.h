// LoginDialog.h

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString getUsername() const;
    QString getUserRole() const; // 新增

private slots:
    void on_loginButton_clicked();

private:
    Ui::LoginDialog *ui;
    QString hashPassword(const QString &password) const;

    QString userRole; // 新增
};

#endif // LOGINDIALOG_H
