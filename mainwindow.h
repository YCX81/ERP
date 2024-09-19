#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QItemSelection>
#include "materialdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();           // 添加物料按钮槽函数
    void on_editButton_clicked();          // 编辑物料按钮槽函数
    void on_deleteButton_clicked();        // 删除物料按钮槽函数
    void on_exportButton_clicked();        // 导出Excel按钮槽函数
    void on_loginButton_clicked();         // 登录按钮槽函数
    void on_registerButton_clicked();      // 注册按钮槽函数
    void on_logoutButton_clicked();        // 注销按钮槽函数
    void on_switchUserButton_clicked();    // 切换用户按钮槽函数
    void onMaterialTableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected); // 表格选中行更改槽函数

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QString currentUser;
    MaterialDialog *materialDialog;
    void updateLoginStatus();   // 更新登录状态的函数
    void refreshMaterialTable(); // 刷新物料表格的函数
    void setMaintainer();        // 设置当前用户为物料维护人的函数
};

#endif // MAINWINDOW_H
