# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/workspace/log

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/log/build

# Include any dependencies generated for this target.
include test/CMakeFiles/serv.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/serv.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/serv.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/serv.dir/flags.make

test/CMakeFiles/serv.dir/serv.cpp.o: test/CMakeFiles/serv.dir/flags.make
test/CMakeFiles/serv.dir/serv.cpp.o: ../test/serv.cpp
test/CMakeFiles/serv.dir/serv.cpp.o: test/CMakeFiles/serv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/log/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/serv.dir/serv.cpp.o"
	cd /root/workspace/log/build/test && /opt/rh/devtoolset-11/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/serv.dir/serv.cpp.o -MF CMakeFiles/serv.dir/serv.cpp.o.d -o CMakeFiles/serv.dir/serv.cpp.o -c /root/workspace/log/test/serv.cpp

test/CMakeFiles/serv.dir/serv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serv.dir/serv.cpp.i"
	cd /root/workspace/log/build/test && /opt/rh/devtoolset-11/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/log/test/serv.cpp > CMakeFiles/serv.dir/serv.cpp.i

test/CMakeFiles/serv.dir/serv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serv.dir/serv.cpp.s"
	cd /root/workspace/log/build/test && /opt/rh/devtoolset-11/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/log/test/serv.cpp -o CMakeFiles/serv.dir/serv.cpp.s

# Object files for target serv
serv_OBJECTS = \
"CMakeFiles/serv.dir/serv.cpp.o"

# External object files for target serv
serv_EXTERNAL_OBJECTS =

test/serv: test/CMakeFiles/serv.dir/serv.cpp.o
test/serv: test/CMakeFiles/serv.dir/build.make
test/serv: lib/libSu.so
test/serv: test/CMakeFiles/serv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/log/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable serv"
	cd /root/workspace/log/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/serv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/serv.dir/build: test/serv
.PHONY : test/CMakeFiles/serv.dir/build

test/CMakeFiles/serv.dir/clean:
	cd /root/workspace/log/build/test && $(CMAKE_COMMAND) -P CMakeFiles/serv.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/serv.dir/clean

test/CMakeFiles/serv.dir/depend:
	cd /root/workspace/log/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/log /root/workspace/log/test /root/workspace/log/build /root/workspace/log/build/test /root/workspace/log/build/test/CMakeFiles/serv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/serv.dir/depend

