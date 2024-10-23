# Install script for directory: C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ERP")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Qt/Tools/mingw1120_64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Release/3rdparty/QXlsx/libQXlsxQt6.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/QXlsxQt6" TYPE FILE FILES
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxabstractooxmlfile.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxabstractsheet.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxabstractsheet_p.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxcellformula.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxcell.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxcelllocation.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxcellrange.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxcellreference.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxchart.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxchartsheet.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxconditionalformatting.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxdatavalidation.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxdatetype.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxdocument.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxformat.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxglobal.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxrichstring.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxworkbook.h"
    "C:/Users/ycx/Documents/QTProjects/ERP/3rdparty/QXlsx/header/xlsxworksheet.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Release/3rdparty/QXlsx/CMakeFiles/QXlsx.dir/install-cxx-module-bmi-Release.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QXlsxQt6/QXlsxQt6Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QXlsxQt6/QXlsxQt6Targets.cmake"
         "C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Release/3rdparty/QXlsx/CMakeFiles/Export/5e1a71f991ec0867fe453527b0963803/QXlsxQt6Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QXlsxQt6/QXlsxQt6Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QXlsxQt6/QXlsxQt6Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QXlsxQt6" TYPE FILE FILES "C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Release/3rdparty/QXlsx/CMakeFiles/Export/5e1a71f991ec0867fe453527b0963803/QXlsxQt6Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QXlsxQt6" TYPE FILE FILES "C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Release/3rdparty/QXlsx/CMakeFiles/Export/5e1a71f991ec0867fe453527b0963803/QXlsxQt6Targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QXlsxQt6" TYPE FILE FILES
    "C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Release/3rdparty/QXlsx/QXlsxQt6Config.cmake"
    "C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Release/3rdparty/QXlsx/QXlsxQt6ConfigVersion.cmake"
    )
endif()

