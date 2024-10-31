# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "3rdparty\\QXlsx\\CMakeFiles\\QXlsx_autogen.dir\\AutogenUsed.txt"
  "3rdparty\\QXlsx\\CMakeFiles\\QXlsx_autogen.dir\\ParseCache.txt"
  "3rdparty\\QXlsx\\QXlsx_autogen"
  "CMakeFiles\\ERP_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ERP_autogen.dir\\ParseCache.txt"
  "ERP_autogen"
  )
endif()
