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

# Utility rule file for QXlsx_autogen.

# Include any custom commands dependencies for this target.
include 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/progress.make

3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen: 3rdparty/QXlsx-1.4.9/QXlsx/QXlsx_autogen/timestamp

3rdparty/QXlsx-1.4.9/QXlsx/QXlsx_autogen/timestamp: C:/Qt/6.7.2/mingw_64/./bin/moc.exe
3rdparty/QXlsx-1.4.9/QXlsx/QXlsx_autogen/timestamp: C:/Qt/6.7.2/mingw_64/./bin/uic.exe
3rdparty/QXlsx-1.4.9/QXlsx/QXlsx_autogen/timestamp: 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target QXlsx"
	cd /d C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\3rdparty\QXlsx-1.4.9\QXlsx && C:\Qt\Tools\CMake_64\bin\cmake.exe -E cmake_autogen C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Debug/3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/AutogenInfo.json Debug
	cd /d C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\3rdparty\QXlsx-1.4.9\QXlsx && C:\Qt\Tools\CMake_64\bin\cmake.exe -E touch C:/Users/ycx/Documents/QTProjects/ERP/build/Desktop_Qt_6_7_2_MinGW_64_bit-Debug/3rdparty/QXlsx-1.4.9/QXlsx/QXlsx_autogen/timestamp

QXlsx_autogen: 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen
QXlsx_autogen: 3rdparty/QXlsx-1.4.9/QXlsx/QXlsx_autogen/timestamp
QXlsx_autogen: 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/build.make
.PHONY : QXlsx_autogen

# Rule to build all files generated by this target.
3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/build: QXlsx_autogen
.PHONY : 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/build

3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/clean:
	cd /d C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\3rdparty\QXlsx-1.4.9\QXlsx && $(CMAKE_COMMAND) -P CMakeFiles\QXlsx_autogen.dir\cmake_clean.cmake
.PHONY : 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/clean

3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\ycx\Documents\QTProjects\ERP C:\Users\ycx\Documents\QTProjects\ERP\3rdparty\QXlsx-1.4.9\QXlsx C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\3rdparty\QXlsx-1.4.9\QXlsx C:\Users\ycx\Documents\QTProjects\ERP\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug\3rdparty\QXlsx-1.4.9\QXlsx\CMakeFiles\QXlsx_autogen.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : 3rdparty/QXlsx-1.4.9/QXlsx/CMakeFiles/QXlsx_autogen.dir/depend

