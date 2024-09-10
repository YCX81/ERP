#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonLoginMain_clicked();
    void handleLoginDialogAccepted();
    void handleAddMaterial();
    void handleRemoveMaterial();
    void handleEditMaterial();
    void handleRegisterUser();

private:
    Ui::MainWindow *ui;
    QDialog *loginDialog;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QCheckBox *rememberMeCheckBox;
    QSqlTableModel *materialModel;

    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
