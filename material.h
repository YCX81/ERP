#ifndef MATERIAL_H
#define MATERIAL_H

<<<<<<< HEAD
#include <QSqlTableModel>

extern QSqlTableModel *materialModel; // 声明为外部变量
=======
#include <QStandardItemModel>

extern QStandardItemModel *materialModel; // 声明为外部变量
>>>>>>> recovery-branch

void addMaterial();
void removeMaterial();
void editMaterial();

#endif // MATERIAL_H
