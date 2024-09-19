#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QByteArray>

namespace Ui {
class MaterialDialog;
}

class MaterialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialDialog(QWidget *parent = nullptr);
    ~MaterialDialog();

    void setMaterialData(const QSqlRecord &record);  // 设置物料数据
    QSqlRecord getMaterialData() const;  // 获取物料数据

private slots:
    void on_uploadDrawingButton_clicked();  // 上传图纸按钮槽函数
    void on_downloadDrawingButton_clicked();  // 下载图纸按钮槽函数
    void on_uploadPhotoButton_clicked();  // 上传实物照片按钮槽函数
    void on_downloadPhotoButton_clicked();  // 下载实物照片按钮槽函数
    void on_uploadSpecificationButton_clicked();  // 上传规格书按钮槽函数
    void on_downloadSpecificationButton_clicked();  // 下载规格书按钮槽函数
    void on_saveButton_clicked();  // 保存按钮槽函数

private:
    Ui::MaterialDialog *ui;
    QByteArray photoData;  // 实物照片数据
    QByteArray drawingData;  // 图纸数据
    QByteArray specificationData;  // 供应商规格书数据
    QString currentMaintainer;  // 当前维护人
};

#endif // MATERIALDIALOG_H
