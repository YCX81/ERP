#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "materialdialog.h"
#include <QStack>
#include <QVariant>

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
    void on_addMaterialButton_clicked();
    void on_editMaterialButton_clicked();
    void on_undoButton_clicked();
    void loadMaterials(); //

private:
    Ui::MainWindow *ui;
    MaterialDialog *materialDialog;
    QStack<QVariant> undoStack; // 撤销栈，用于存储历史操作
    void addMaterial(const QString &name, const QString &type, int quantity, double price);
    void updateMaterial(int row, const QString &name, const QString &type, int quantity, double price);
};

#endif // MAINWINDOW_H

