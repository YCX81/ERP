#ifndef BOMDIALOG_H
#define BOMDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSqlRecord>

namespace Ui {
class BOMDialog;
}

struct BOMMaterial {
    int materialId;
    QString materialNumber;
    int version;
    QString description;
    QString remarks;
    //QString usageMM;
    int quantity;
    QString source;
    QString supplier;
    double unitPrice;
    double bomPrice;
};

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

private:
    Ui::BOMDialog *ui;
    QStandardItemModel *materialListModel;
    QSqlRecord bomRecord;
    QList<BOMMaterial> bomMaterials;

    void loadMaterials();
    void updateMaterialTable();
    bool validateInput();
    void calculateTotalCost();
};

#endif // BOMDIALOG_H
