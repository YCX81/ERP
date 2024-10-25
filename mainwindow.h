#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QMap>
#include <QItemSelection>
#include <QSqlRelationalTableModel>
#include <QUndoStack>
#include <QUndoCommand>
#include <QSqlRecord>

namespace Ui {
class MainWindow;
}

class QTableView;

// 命令类用于撤销和重做操作
class AddMaterialCommand : public QUndoCommand
{
public:
    AddMaterialCommand(QSqlTableModel *model, const QSqlRecord &record, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    QSqlTableModel *model;
    QSqlRecord record;
    int row;
};

class EditMaterialCommand : public QUndoCommand
{
public:
    EditMaterialCommand(QSqlTableModel *model, int row, const QSqlRecord &oldRecord, const QSqlRecord &newRecord, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    QSqlTableModel *model;
    int row;
    QSqlRecord oldRecord;
    QSqlRecord newRecord;
};

class DeleteMaterialCommand : public QUndoCommand
{
public:
    DeleteMaterialCommand(QSqlTableModel *model, int row, const QSqlRecord &record, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;
private:
    QSqlTableModel *model;
    int row;
    QSqlRecord record;
};

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
    void on_supplierButton_clicked();

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
    void on_redoButton_clicked(); // 新增槽函数
    void on_orderStatusChanged(int orderId, const QString &newStatus);

    // 供应商管理操作
    void on_addSupplierButton_clicked();
    void on_editSupplierButton_clicked();
    void on_deleteSupplierButton_clicked();

    // 搜索功能槽函数
    void on_materialSearchLineEdit_textChanged(const QString &text);
    void on_defectiveSearchLineEdit_textChanged(const QString &text);
    void on_bomSearchLineEdit_textChanged(const QString &text);
    void on_supplierSearchLineEdit_textChanged(const QString &text);

    // 其他辅助函数
    void onMaterialTableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onSupplierSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onBOMSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    Ui::MainWindow *ui;
    QString currentUser;

    // 模型
    QSqlTableModel *materialModel;
    QSqlTableModel *defectiveModel;
    QSqlTableModel *bomModel;
    QSqlTableModel *supplierModel;

    // 代理模型，用于搜索和过滤
    QSortFilterProxyModel *materialProxyModel;
    QSortFilterProxyModel *defectiveProxyModel;
    QSortFilterProxyModel *bomProxyModel;
    QSortFilterProxyModel *supplierProxyModel;

    QMap<QString, QSqlTableModel*> orderModels;
    QMap<QString, QSortFilterProxyModel*> orderProxyModels; // 新增，用于订单的搜索和过滤
    QMap<QString, QTableView*> orderTableViews;

    // 撤销和重做堆栈
    QUndoStack *undoStack;

    // 更新登录状态
    void updateLoginStatus();

    // 显示预览数据
    void displayDataInPreview(const QByteArray &data);

    // 库存处理函数
    void processMaterialInventory(int orderId, const QString &operation);
    void processProductInventory(int orderId, const QString &operation);
};

#endif // MAINWINDOW_H
