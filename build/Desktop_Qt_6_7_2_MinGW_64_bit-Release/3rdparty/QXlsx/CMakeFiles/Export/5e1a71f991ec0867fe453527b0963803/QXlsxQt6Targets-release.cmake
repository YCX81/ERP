#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QXlsx::QXlsx" for configuration "Release"
set_property(TARGET QXlsx::QXlsx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(QXlsx::QXlsx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libQXlsxQt6.a"
  )

list(APPEND _cmake_import_check_targets QXlsx::QXlsx )
list(APPEND _cmake_import_check_files_for_QXlsx::QXlsx "${_IMPORT_PREFIX}/lib/libQXlsxQt6.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
