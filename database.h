#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class Database
{
public:
    static Database& instance();
    bool connectToDatabase(const QString& host, const QString& dbName, const QString& user, const QString& password);
    bool addMaterial(const QString& name, const QString& type, int quantity, double price);
    bool updateMaterial(int id, const QString& name, const QString& type, int quantity, double price);
    bool removeMaterial(int id);
    QSqlQuery getMaterials();

private:
    Database();
    QSqlDatabase db;
    bool executeQuery(const QString& queryStr, const QMap<QString, QVariant>& params);
};

#endif // DATABASE_H
