#ifndef MATERIAL_H
#define MATERIAL_H

#include <QSqlTableModel>

extern QSqlTableModel *materialModel; // 声明为外部变量

void addMaterial();
void removeMaterial();
void editMaterial();

#endif // MATERIAL_H
