#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);

private:
    QLineEdit *usernameLineEdit; // 用户名输入框
    QLineEdit *passwordLineEdit; // 密码输入框
    QPushButton *loginButton;    // 登录按钮
    QPushButton *cancelButton;   // 取消按钮
};

#endif // LOGINDIALOG_H
