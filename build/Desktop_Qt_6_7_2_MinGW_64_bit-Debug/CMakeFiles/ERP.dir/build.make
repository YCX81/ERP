# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Qt\Tools\CMake_64\bin\cmake.exe

# The command to remove a file.
RM = C:\Qt\Tools\CMake_64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\ycx\Documents\QTProjects\ERP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug

# Include any dependencies generated for this target.
include CMakeFiles/ERP.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ERP.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ERP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ERP.dir/flags.make

ERP_autogen/timestamp: C:/Qt/6.7.2/mingw_64/./bin/moc.exe
ERP_autogen/timestamp: C:/Qt/6.7.2/mingw_64/./bin/uic.exe
ERP_autogen/timestamp: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target ERP"
	C:\Qt\Tools\CMake_64\bin\cmake.exe -E cmake_autogen C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Debug/CMakeFiles/ERP_autogen.dir/AutogenInfo.json Debug
	C:\Qt\Tools\CMake_64\bin\cmake.exe -E touch C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Debug/ERP_autogen/timestamp

CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj: ERP_autogen/mocs_compilation.cpp
CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\ERP.dir\ERP_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\ERP.dir\ERP_autogen\mocs_compilation.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\ERP_autogen\mocs_compilation.cpp

CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\ERP_autogen\mocs_compilation.cpp > CMakeFiles\ERP.dir\ERP_autogen\mocs_compilation.cpp.i

CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\ERP_autogen\mocs_compilation.cpp -o CMakeFiles\ERP.dir\ERP_autogen\mocs_compilation.cpp.s

CMakeFiles/ERP.dir/main.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/main.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/main.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/main.cpp
CMakeFiles/ERP.dir/main.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ERP.dir/main.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/main.cpp.obj -MF CMakeFiles\ERP.dir\main.cpp.obj.d -o CMakeFiles\ERP.dir\main.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\main.cpp

CMakeFiles/ERP.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/main.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\main.cpp > CMakeFiles\ERP.dir\main.cpp.i

CMakeFiles/ERP.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/main.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\main.cpp -o CMakeFiles\ERP.dir\main.cpp.s

CMakeFiles/ERP.dir/mainwindow.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/mainwindow.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/mainwindow.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/mainwindow.cpp
CMakeFiles/ERP.dir/mainwindow.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ERP.dir/mainwindow.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/mainwindow.cpp.obj -MF CMakeFiles\ERP.dir\mainwindow.cpp.obj.d -o CMakeFiles\ERP.dir\mainwindow.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\mainwindow.cpp

CMakeFiles/ERP.dir/mainwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/mainwindow.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\mainwindow.cpp > CMakeFiles\ERP.dir\mainwindow.cpp.i

CMakeFiles/ERP.dir/mainwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/mainwindow.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\mainwindow.cpp -o CMakeFiles\ERP.dir\mainwindow.cpp.s

CMakeFiles/ERP.dir/logindialog.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/logindialog.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/logindialog.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/logindialog.cpp
CMakeFiles/ERP.dir/logindialog.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ERP.dir/logindialog.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/logindialog.cpp.obj -MF CMakeFiles\ERP.dir\logindialog.cpp.obj.d -o CMakeFiles\ERP.dir\logindialog.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\logindialog.cpp

CMakeFiles/ERP.dir/logindialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/logindialog.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\logindialog.cpp > CMakeFiles\ERP.dir\logindialog.cpp.i

CMakeFiles/ERP.dir/logindialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/logindialog.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\logindialog.cpp -o CMakeFiles\ERP.dir\logindialog.cpp.s

CMakeFiles/ERP.dir/materialdialog.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/materialdialog.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/materialdialog.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/materialdialog.cpp
CMakeFiles/ERP.dir/materialdialog.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ERP.dir/materialdialog.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/materialdialog.cpp.obj -MF CMakeFiles\ERP.dir\materialdialog.cpp.obj.d -o CMakeFiles\ERP.dir\materialdialog.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\materialdialog.cpp

CMakeFiles/ERP.dir/materialdialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/materialdialog.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\materialdialog.cpp > CMakeFiles\ERP.dir\materialdialog.cpp.i

CMakeFiles/ERP.dir/materialdialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/materialdialog.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\materialdialog.cpp -o CMakeFiles\ERP.dir\materialdialog.cpp.s

CMakeFiles/ERP.dir/registerdialog.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/registerdialog.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/registerdialog.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/registerdialog.cpp
CMakeFiles/ERP.dir/registerdialog.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/ERP.dir/registerdialog.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/registerdialog.cpp.obj -MF CMakeFiles\ERP.dir\registerdialog.cpp.obj.d -o CMakeFiles\ERP.dir\registerdialog.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\registerdialog.cpp

CMakeFiles/ERP.dir/registerdialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/registerdialog.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\registerdialog.cpp > CMakeFiles\ERP.dir\registerdialog.cpp.i

CMakeFiles/ERP.dir/registerdialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/registerdialog.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\registerdialog.cpp -o CMakeFiles\ERP.dir\registerdialog.cpp.s

CMakeFiles/ERP.dir/bomdialog.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/bomdialog.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/bomdialog.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/bomdialog.cpp
CMakeFiles/ERP.dir/bomdialog.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/ERP.dir/bomdialog.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/bomdialog.cpp.obj -MF CMakeFiles\ERP.dir\bomdialog.cpp.obj.d -o CMakeFiles\ERP.dir\bomdialog.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\bomdialog.cpp

CMakeFiles/ERP.dir/bomdialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/bomdialog.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\bomdialog.cpp > CMakeFiles\ERP.dir\bomdialog.cpp.i

CMakeFiles/ERP.dir/bomdialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/bomdialog.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\bomdialog.cpp -o CMakeFiles\ERP.dir\bomdialog.cpp.s

CMakeFiles/ERP.dir/orderdialog.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/orderdialog.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/orderdialog.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/orderdialog.cpp
CMakeFiles/ERP.dir/orderdialog.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/ERP.dir/orderdialog.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/orderdialog.cpp.obj -MF CMakeFiles\ERP.dir\orderdialog.cpp.obj.d -o CMakeFiles\ERP.dir\orderdialog.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\orderdialog.cpp

CMakeFiles/ERP.dir/orderdialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/orderdialog.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\orderdialog.cpp > CMakeFiles\ERP.dir\orderdialog.cpp.i

CMakeFiles/ERP.dir/orderdialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/orderdialog.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\orderdialog.cpp -o CMakeFiles\ERP.dir\orderdialog.cpp.s

CMakeFiles/ERP.dir/defectivedialog.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/defectivedialog.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/defectivedialog.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/defectivedialog.cpp
CMakeFiles/ERP.dir/defectivedialog.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/ERP.dir/defectivedialog.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/defectivedialog.cpp.obj -MF CMakeFiles\ERP.dir\defectivedialog.cpp.obj.d -o CMakeFiles\ERP.dir\defectivedialog.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\defectivedialog.cpp

CMakeFiles/ERP.dir/defectivedialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/defectivedialog.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\defectivedialog.cpp > CMakeFiles\ERP.dir\defectivedialog.cpp.i

CMakeFiles/ERP.dir/defectivedialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/defectivedialog.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\defectivedialog.cpp -o CMakeFiles\ERP.dir\defectivedialog.cpp.s

CMakeFiles/ERP.dir/supplierdialog.cpp.obj: CMakeFiles/ERP.dir/flags.make
CMakeFiles/ERP.dir/supplierdialog.cpp.obj: CMakeFiles/ERP.dir/includes_CXX.rsp
CMakeFiles/ERP.dir/supplierdialog.cpp.obj: C:/Users/ycx/Documents/QTProjects/ERP/supplierdialog.cpp
CMakeFiles/ERP.dir/supplierdialog.cpp.obj: CMakeFiles/ERP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/ERP.dir/supplierdialog.cpp.obj"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ERP.dir/supplierdialog.cpp.obj -MF CMakeFiles\ERP.dir\supplierdialog.cpp.obj.d -o CMakeFiles\ERP.dir\supplierdialog.cpp.obj -c C:\Users\ycx\Documents\QTProjects\ERP\supplierdialog.cpp

CMakeFiles/ERP.dir/supplierdialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ERP.dir/supplierdialog.cpp.i"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ycx\Documents\QTProjects\ERP\supplierdialog.cpp > CMakeFiles\ERP.dir\supplierdialog.cpp.i

CMakeFiles/ERP.dir/supplierdialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ERP.dir/supplierdialog.cpp.s"
	C:\Qt\Tools\mingw1120_64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ycx\Documents\QTProjects\ERP\supplierdialog.cpp -o CMakeFiles\ERP.dir\supplierdialog.cpp.s

# Object files for target ERP
ERP_OBJECTS = \
"CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/ERP.dir/main.cpp.obj" \
"CMakeFiles/ERP.dir/mainwindow.cpp.obj" \
"CMakeFiles/ERP.dir/logindialog.cpp.obj" \
"CMakeFiles/ERP.dir/materialdialog.cpp.obj" \
"CMakeFiles/ERP.dir/registerdialog.cpp.obj" \
"CMakeFiles/ERP.dir/bomdialog.cpp.obj" \
"CMakeFiles/ERP.dir/orderdialog.cpp.obj" \
"CMakeFiles/ERP.dir/defectivedialog.cpp.obj" \
"CMakeFiles/ERP.dir/supplierdialog.cpp.obj"

# External object files for target ERP
ERP_EXTERNAL_OBJECTS =

ERP.exe: CMakeFiles/ERP.dir/ERP_autogen/mocs_compilation.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/main.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/mainwindow.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/logindialog.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/materialdialog.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/registerdialog.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/bomdialog.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/orderdialog.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/defectivedialog.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/supplierdialog.cpp.obj
ERP.exe: CMakeFiles/ERP.dir/build.make
ERP.exe: C:/Qt/6.7.2/mingw_64/lib/libQt6Widgets.a
ERP.exe: C:/Qt/6.7.2/mingw_64/lib/libQt6Sql.a
ERP.exe: 3rdparty/QXlsx/libQXlsxQt6.a
ERP.exe: C:/Qt/6.7.2/mingw_64/lib/libQt6Pdf.a
ERP.exe: C:/Qt/6.7.2/mingw_64/lib/libQt6Gui.a
ERP.exe: C:/Qt/6.7.2/mingw_64/lib/libQt6Core.a
ERP.exe: CMakeFiles/ERP.dir/linkLibs.rsp
ERP.exe: CMakeFiles/ERP.dir/objects1.rsp
ERP.exe: CMakeFiles/ERP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable ERP.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ERP.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ERP.dir/build: ERP.exe
.PHONY : CMakeFiles/ERP.dir/build

CMakeFiles/ERP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ERP.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ERP.dir/clean

CMakeFiles/ERP.dir/depend: ERP_autogen/timestamp
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\ycx\Documents\QTProjects\ERP C:\Users\ycx\Documents\QTProjects\ERP C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles\ERP.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ERP.dir/depend
