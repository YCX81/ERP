QT += core gui sql  # 添加 MySQL 和核心 GUI 支持

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 使代码无法编译如果使用了弃用的API
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # 禁用所有在 Qt 6.0.0 之前的弃用API

# 源文件
SOURCES += \
    database.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    material.cpp \
    user.cpp

# 头文件
HEADERS += \
    database.h \
    logindialog.h \
    mainwindow.h \
    material.h \
    user.h

# UI 文件
FORMS += \
    mainwindow.ui

# 翻译文件
TRANSLATIONS += \
    ERP_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# 添加 MySQL 包含目录
INCLUDEPATH += "C:/Program Files/MySQL/MySQL Server 8.0/include"

# 添加 MySQL 库路径和链接的库
LIBS += -L"C:/Program Files/MySQL/MySQL Server 8.0/lib" -lmysqlclient

# 默认的部署规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
