#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QMessageBox>

extern QSqlDatabase db;

bool connectToDatabase();

#endif // DATABASE_H
