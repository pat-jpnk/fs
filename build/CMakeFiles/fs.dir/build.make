# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /snap/cmake/1391/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1391/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/patrick/Programming/fs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/patrick/Programming/fs/build

# Include any dependencies generated for this target.
include CMakeFiles/fs.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/fs.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/fs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fs.dir/flags.make

CMakeFiles/fs.dir/app/fs.c.o: CMakeFiles/fs.dir/flags.make
CMakeFiles/fs.dir/app/fs.c.o: /home/patrick/Programming/fs/app/fs.c
CMakeFiles/fs.dir/app/fs.c.o: CMakeFiles/fs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/patrick/Programming/fs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/fs.dir/app/fs.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/fs.dir/app/fs.c.o -MF CMakeFiles/fs.dir/app/fs.c.o.d -o CMakeFiles/fs.dir/app/fs.c.o -c /home/patrick/Programming/fs/app/fs.c

CMakeFiles/fs.dir/app/fs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/fs.dir/app/fs.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/patrick/Programming/fs/app/fs.c > CMakeFiles/fs.dir/app/fs.c.i

CMakeFiles/fs.dir/app/fs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/fs.dir/app/fs.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/patrick/Programming/fs/app/fs.c -o CMakeFiles/fs.dir/app/fs.c.s

CMakeFiles/fs.dir/app/operations.c.o: CMakeFiles/fs.dir/flags.make
CMakeFiles/fs.dir/app/operations.c.o: /home/patrick/Programming/fs/app/operations.c
CMakeFiles/fs.dir/app/operations.c.o: CMakeFiles/fs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/patrick/Programming/fs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/fs.dir/app/operations.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/fs.dir/app/operations.c.o -MF CMakeFiles/fs.dir/app/operations.c.o.d -o CMakeFiles/fs.dir/app/operations.c.o -c /home/patrick/Programming/fs/app/operations.c

CMakeFiles/fs.dir/app/operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/fs.dir/app/operations.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/patrick/Programming/fs/app/operations.c > CMakeFiles/fs.dir/app/operations.c.i

CMakeFiles/fs.dir/app/operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/fs.dir/app/operations.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/patrick/Programming/fs/app/operations.c -o CMakeFiles/fs.dir/app/operations.c.s

# Object files for target fs
fs_OBJECTS = \
"CMakeFiles/fs.dir/app/fs.c.o" \
"CMakeFiles/fs.dir/app/operations.c.o"

# External object files for target fs
fs_EXTERNAL_OBJECTS =

/home/patrick/Programming/fs/bin/fs: CMakeFiles/fs.dir/app/fs.c.o
/home/patrick/Programming/fs/bin/fs: CMakeFiles/fs.dir/app/operations.c.o
/home/patrick/Programming/fs/bin/fs: CMakeFiles/fs.dir/build.make
/home/patrick/Programming/fs/bin/fs: CMakeFiles/fs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/patrick/Programming/fs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable /home/patrick/Programming/fs/bin/fs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fs.dir/build: /home/patrick/Programming/fs/bin/fs
.PHONY : CMakeFiles/fs.dir/build

CMakeFiles/fs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fs.dir/clean

CMakeFiles/fs.dir/depend:
	cd /home/patrick/Programming/fs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/patrick/Programming/fs /home/patrick/Programming/fs /home/patrick/Programming/fs/build /home/patrick/Programming/fs/build /home/patrick/Programming/fs/build/CMakeFiles/fs.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/fs.dir/depend

