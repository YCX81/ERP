#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QSqlRecord>

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

    bool validateInput();
};

#endif // MATERIALDIALOG_H
