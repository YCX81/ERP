#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QByteArray>
#include <QList>
#include <QMetaType>

namespace Ui {
class MaterialDialog;
}

// 定义 CategoryInfo 结构体，用于存储类别编号、类别名称和字母代码
struct CategoryInfo {
    int code;          // 类别编号
    QString name;      // 类别中文名称
    QString letterCode;  // 类别对应的两位字母代码
};

Q_DECLARE_METATYPE(CategoryInfo)  // 注册 CategoryInfo 类型

class MaterialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialDialog(const QString &currentUser, QWidget *parent = nullptr);
    ~MaterialDialog();

    void setMaterialData(const QSqlRecord &record);  // 设置物料数据
    QSqlRecord getMaterialData() const;              // 获取物料数据

    // 启用仅编辑不良品数量的模式
    void enableDefectiveQuantityEditing(bool enable);

private slots:
    void on_uploadDrawingButton_clicked();        // 上传图纸按钮槽函数
    void on_downloadDrawingButton_clicked();      // 下载图纸按钮槽函数
    void on_uploadPhotoButton_clicked();          // 上传实物照片按钮槽函数
    void on_downloadPhotoButton_clicked();        // 下载实物照片按钮槽函数
    void on_saveButton_clicked();                 // 保存按钮槽函数
    void on_materialNumberGenerateButton_clicked(); // 生成物料号按钮槽函数

private:
    Ui::MaterialDialog *ui;
    QByteArray photoData;     // 实物照片数据
    QByteArray drawingData;   // 图纸数据
    QString currentMaintainer; // 当前维护人

    bool defectiveQuantityEditingEnabled; // 是否仅编辑不良品数量

    bool validateInput(); // 验证输入数据的有效性
};

#endif // MATERIALDIALOG_H
