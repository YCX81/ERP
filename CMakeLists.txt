cmake_minimum_required(VERSION 3.16)

project(ERP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Sql LinguistTools)

set(TS_FILES ERP_zh_CN.ts)

# 源文件列表
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    logindialog.cpp
    materialdialog.cpp
    registerdialog.cpp
    mainwindow.h
    logindialog.h
    main.h
    materialdialog.h
    registerdialog.h
    mainwindow.ui
    logindialog.ui
    materialdialog.ui
    registerdialog.ui
    ${TS_FILES}
)

# 添加可执行文件
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ERP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        bomdialog.h bomdialog.cpp
        orderdialog.h orderdialog.cpp
        orderdialog.ui
        BOMdialog.ui
        defectivedialog.h defectivedialog.cpp
        supplierdialog.h supplierdialog.cpp
        supplierdialog.ui
        selectmaterialdialog.h selectmaterialdialog.cpp
        selectmaterialdialog.ui
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(ERP SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ERP
            ${PROJECT_SOURCES}
        )
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 链接Qt库
target_link_libraries(ERP PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# 查找 MySQL 库
find_library(MySQL_LIBRARY NAMES libmysql PATHS "C:/Program Files/MySQL/MySQL Server 8.0/lib")

# 包含 MySQL 头文件路径
include_directories("C:/Program Files/MySQL/MySQL Server 8.0/include")

# 设置编译选项
add_definitions(-DQT_BUILD_MYSQL_DRIVER)

# 添加 QXlsx 库的头文件路径
add_subdirectory(3rdparty/QXlsx)
target_link_libraries(ERP PRIVATE QXlsx::QXlsx)

#添加 QPDF
find_package(Qt6 REQUIRED COMPONENTS Pdf)
target_link_libraries(ERP PRIVATE Qt6::Pdf)


# 安装目标设置
install(TARGETS ERP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ERP)
endif()
