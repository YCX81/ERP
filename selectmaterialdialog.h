#ifndef SELECTMATERIALDIALOG_H
#define SELECTMATERIALDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QStandardItemModel>

namespace Ui {
class SelectMaterialDialog;
}

class SelectMaterialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectMaterialDialog(QWidget *parent = nullptr);
    ~SelectMaterialDialog();

    QSqlRecord getSelectedMaterial() const;

private slots:
    void on_searchButton_clicked();
    void on_materialTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::SelectMaterialDialog *ui;
    QStandardItemModel *materialModel;
    QSqlRecord selectedMaterial;
    void loadMaterials(const QString &filter = "");
};

#endif // SELECTMATERIALDIALOG_H
