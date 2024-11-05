#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QSqlRecord>
#include <QFileInfo> // 添加此行以支持 QFileInfo

struct CategoryInfo {
    int category_code;
    QString category_name;
    QString letter_code;

    bool operator==(const CategoryInfo &other) const {
        return category_code == other.category_code &&
               category_name == other.category_name &&
               letter_code == other.letter_code;
    }
};

Q_DECLARE_METATYPE(CategoryInfo)

namespace Ui {
class MaterialDialog;
}

class MaterialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialDialog(const QString &currentUser, QWidget *parent = nullptr);
    ~MaterialDialog();

    void setMaterialData(const QSqlRecord &record);
    QSqlRecord getMaterialData() const;
    void enableDefectiveQuantityEditing(bool enable);

private slots:
    void on_generateMaterialNumberButton_clicked();
    void on_uploadDrawingButton_clicked();
    void on_downloadDrawingButton_clicked();
    void on_uploadPhotoButton_clicked();
    void on_downloadPhotoButton_clicked();
    void on_saveButton_clicked();

private:
    Ui::MaterialDialog *ui;
    QString currentMaintainer;
    bool defectiveQuantityEditingEnabled;
    QString drawingPath;
    QString photoPath;
    QString originalMKNumber;

    // 新增成员变量用于标识编辑模式
    bool isEditMode;

    // 新增方法用于生成唯一文件名
    QString uniqueFileName(const QFileInfo &fileInfo) const;

    bool validateInput();

    void setFieldsReadOnly(bool readOnly);
};

#endif // MATERIALDIALOG_H
