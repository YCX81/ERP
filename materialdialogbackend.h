#ifndef MATERIALDIALOGBACKEND_H
#define MATERIALDIALOGBACKEND_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QSqlRecord>

struct CategoryInfo {
    int id;
    QString name;
    QString letterCode;
};
Q_DECLARE_METATYPE(CategoryInfo)

class MaterialDialogBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> categoryList READ getCategoryList NOTIFY categoryListChanged)
    Q_PROPERTY(QList<QVariant> supplierList READ getSupplierList NOTIFY supplierListChanged)
    Q_PROPERTY(QString currentMaintainer READ currentMaintainer WRITE setCurrentMaintainer NOTIFY currentMaintainerChanged)

public:
    explicit MaterialDialogBackend(const QString &currentUser, QObject *parent = nullptr);

    QList<QVariant> getCategoryList() const;
    QList<QVariant> getSupplierList() const;

    QString currentMaintainer() const;
    void setCurrentMaintainer(const QString &maintainer);

    Q_INVOKABLE QString generateMaterialNumber(const QString &categoryLetterCode, const QString &supplierID, int versionNumber);
    Q_INVOKABLE bool uploadFile(const QString &filePath, QByteArray &data);
    Q_INVOKABLE bool downloadFile(const QString &filePath, const QByteArray &data);
    Q_INVOKABLE bool saveMaterialData(const QVariantMap &recordMap);
    Q_INVOKABLE bool validateInput(const QVariantMap &recordMap, QString &errorMessage);

signals:
    void categoryListChanged();
    void supplierListChanged();
    void currentMaintainerChanged();

private:
    QList<CategoryInfo> m_categoryList;
    QList<QVariant> m_supplierList;
    QString m_currentMaintainer;

    void initializeCategories();
    void initializeSuppliers();

    QByteArray drawingData;
    QByteArray photoData;

    bool saveMaterialData(const QSqlRecord &record);
    QSqlRecord getMaterialRecord(const QVariantMap &recordMap);
};

#endif // MATERIALDIALOGBACKEND_H
