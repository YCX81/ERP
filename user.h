#ifndef USER_H
#define USER_H

#include <QString>

bool authenticateUser(const QString &username, const QString &password);
bool registerUser(const QString &username, const QString &password);

#endif // USER_H
