# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = J:\log

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = J:\log\build

# Include any dependencies generated for this target.
include CMakeFiles/log.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/log.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/log.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/log.dir/flags.make

CMakeFiles/log.dir/main.cpp.obj: CMakeFiles/log.dir/flags.make
CMakeFiles/log.dir/main.cpp.obj: CMakeFiles/log.dir/includes_CXX.rsp
CMakeFiles/log.dir/main.cpp.obj: ../main.cpp
CMakeFiles/log.dir/main.cpp.obj: CMakeFiles/log.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=J:\log\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/log.dir/main.cpp.obj"
	C:\Users\75108\Desktop\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/log.dir/main.cpp.obj -MF CMakeFiles\log.dir\main.cpp.obj.d -o CMakeFiles\log.dir\main.cpp.obj -c J:\log\main.cpp

CMakeFiles/log.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/log.dir/main.cpp.i"
	C:\Users\75108\Desktop\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E J:\log\main.cpp > CMakeFiles\log.dir\main.cpp.i

CMakeFiles/log.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/log.dir/main.cpp.s"
	C:\Users\75108\Desktop\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S J:\log\main.cpp -o CMakeFiles\log.dir\main.cpp.s

CMakeFiles/log.dir/lib/log.cpp.obj: CMakeFiles/log.dir/flags.make
CMakeFiles/log.dir/lib/log.cpp.obj: CMakeFiles/log.dir/includes_CXX.rsp
CMakeFiles/log.dir/lib/log.cpp.obj: ../lib/log.cpp
CMakeFiles/log.dir/lib/log.cpp.obj: CMakeFiles/log.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=J:\log\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/log.dir/lib/log.cpp.obj"
	C:\Users\75108\Desktop\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/log.dir/lib/log.cpp.obj -MF CMakeFiles\log.dir\lib\log.cpp.obj.d -o CMakeFiles\log.dir\lib\log.cpp.obj -c J:\log\lib\log.cpp

CMakeFiles/log.dir/lib/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/log.dir/lib/log.cpp.i"
	C:\Users\75108\Desktop\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E J:\log\lib\log.cpp > CMakeFiles\log.dir\lib\log.cpp.i

CMakeFiles/log.dir/lib/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/log.dir/lib/log.cpp.s"
	C:\Users\75108\Desktop\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S J:\log\lib\log.cpp -o CMakeFiles\log.dir\lib\log.cpp.s

# Object files for target log
log_OBJECTS = \
"CMakeFiles/log.dir/main.cpp.obj" \
"CMakeFiles/log.dir/lib/log.cpp.obj"

# External object files for target log
log_EXTERNAL_OBJECTS =

log.exe: CMakeFiles/log.dir/main.cpp.obj
log.exe: CMakeFiles/log.dir/lib/log.cpp.obj
log.exe: CMakeFiles/log.dir/build.make
log.exe: CMakeFiles/log.dir/linklibs.rsp
log.exe: CMakeFiles/log.dir/objects1.rsp
log.exe: CMakeFiles/log.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=J:\log\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable log.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\log.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/log.dir/build: log.exe
.PHONY : CMakeFiles/log.dir/build

CMakeFiles/log.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\log.dir\cmake_clean.cmake
.PHONY : CMakeFiles/log.dir/clean

CMakeFiles/log.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" J:\log J:\log J:\log\build J:\log\build J:\log\build\CMakeFiles\log.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/log.dir/depend

