#include "database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Database::Database()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
}

Database& Database::instance()
{
    static Database instance;
    return instance;
}

bool Database::connectToDatabase(const QString& host, const QString& dbName, const QString& user, const QString& password)
{
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << "Database error: " << db.lastError().text();
        return false;
    }
    return true;
}

bool Database::addMaterial(const QString& name, const QString& type, int quantity, double price)
{
    QString queryStr = "INSERT INTO materials (name, type, quantity, price) VALUES (:name, :type, :quantity, :price)";
    QMap<QString, QVariant> params;
    params[":name"] = name;
    params[":type"] = type;
    params[":quantity"] = quantity;
    params[":price"] = price;
    return executeQuery(queryStr, params);
}

bool Database::updateMaterial(int id, const QString& name, const QString& type, int quantity, double price)
{
    QString queryStr = "UPDATE materials SET name = :name, type = :type, quantity = :quantity, price = :price WHERE id = :id";
    QMap<QString, QVariant> params;
    params[":id"] = id;
    params[":name"] = name;
    params[":type"] = type;
    params[":quantity"] = quantity;
    params[":price"] = price;
    return executeQuery(queryStr, params);
}

bool Database::removeMaterial(int id)
{
    QString queryStr = "DELETE FROM materials WHERE id = :id";
    QMap<QString, QVariant> params;
    params[":id"] = id;
    return executeQuery(queryStr, params);
}

QSqlQuery Database::getMaterials()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM materials");
    return query;
}

bool Database::executeQuery(const QString& queryStr, const QMap<QString, QVariant>& params)
{
    QSqlQuery query(db);
    query.prepare(queryStr);

    for (auto it = params.begin(); it != params.end(); ++it) {
        query.bindValue(it.key(), it.value());
    }

    if (!query.exec()) {
        qDebug() << "SQL error: " << query.lastError().text();
        return false;
    }
    return true;
}
