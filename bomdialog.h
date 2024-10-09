#ifndef BOMDIALOG_H
#define BOMDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QStandardItemModel>

namespace Ui {
class BOMDialog;
}

class BOMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BOMDialog(QWidget *parent = nullptr);
    ~BOMDialog();

    void setBOMData(const QSqlRecord &record);
    QSqlRecord getBOMData() const;

private slots:
    void on_addMaterialButton_clicked();
    void on_removeMaterialButton_clicked();
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    bool validateInput();

private:
    Ui::BOMDialog *ui;
    QSqlRecord bomRecord;
    QStandardItemModel *materialListModel;
};

#endif // BOMDIALOG_H
