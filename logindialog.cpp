#include "logindialog.h"
#include <QVBoxLayout>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("登录");

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("用户名");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("密码");
    passwordLineEdit->setEchoMode(QLineEdit::Password); // 密码模式

    loginButton = new QPushButton("登录", this);
    cancelButton = new QPushButton("取消", this);

    // 布局设置
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
    layout->addWidget(cancelButton);

    setLayout(layout);

    // 连接信号与槽
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::accept); // 点击登录，关闭对话框
    connect(cancelButton, &QPushButton::clicked, this, &LoginDialog::reject); // 点击取消，关闭对话框
}
