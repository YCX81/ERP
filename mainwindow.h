#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMap>
#include <QItemSelection>
#include <QSqlRelationalTableModel>

namespace Ui {
class MainWindow;
}

class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 用户操作
    void on_registerButton_clicked();
    void on_loginButton_clicked();
    void on_logoutButton_clicked();
    void on_switchUserButton_clicked();

    // 界面切换
    void on_materialButton_clicked();
    void on_defectiveButton_clicked();
    void on_bomButton_clicked();
    void on_orderButton_clicked();
    void on_supplierButton_clicked(); // 添加供应商管理按钮的槽函数

    // 物料管理操作
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_exportButton_clicked();
    void on_viewDrawingButton_clicked();
    void on_viewPhotoButton_clicked();

    // 不良品管理操作
    void on_addDefectiveButton_clicked();
    void on_editDefectiveButton_clicked();
    void on_deleteDefectiveButton_clicked();

    // BOM管理操作
    void on_addBOMButton_clicked();
    void on_editBOMButton_clicked();
    void on_deleteBOMButton_clicked();

    // 订单管理操作
    void on_addOrderButton_clicked();
    void on_editOrderButton_clicked();
    void on_deleteOrderButton_clicked();
    void on_undoButton_clicked();
    void on_orderStatusChanged(int orderId, const QString &newStatus);

    // 供应商管理操作
    void on_addSupplierButton_clicked();
    void on_editSupplierButton_clicked();
    void on_deleteSupplierButton_clicked();

    // 其他辅助函数
    void onMaterialTableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onSupplierSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    Ui::MainWindow *ui;
    QString currentUser;

    // 模型
    QSqlTableModel *materialModel;
    QSqlTableModel *defectiveModel;
    QSqlTableModel *bomModel;
    QSqlTableModel *supplierModel; // 添加供应商模型
    QSqlRelationalTableModel *supplierMaterialModel; // 用于显示供应商及其物料

    QMap<QString, QSqlTableModel*> orderModels;
    QMap<QString, QTableView*> orderTableViews;

    // 更新登录状态
    void updateLoginStatus();
    // 显示预览数据
    void displayDataInPreview(const QByteArray &data);
    void processMaterialInventory(int orderId, const QString &operation);
    void processProductInventory(int orderId, const QString &operation);
};

#endif // MAINWINDOW_H
