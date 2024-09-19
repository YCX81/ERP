// registerdialog.h
#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_registerButton_clicked();

private:
    Ui::RegisterDialog *ui;
    QString hashPassword(const QString &password) const;
    bool passwordRequirementsMet(const QString &password) const;
};
#endif // REGISTERDIALOG_H
