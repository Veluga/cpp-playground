# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.17.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.17.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/bin

# Include any dependencies generated for this target.
include CMakeFiles/test_matrix_multiply.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_matrix_multiply.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_matrix_multiply.dir/flags.make

CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.o: CMakeFiles/test_matrix_multiply.dir/flags.make
CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.o: ../app/test_matrix_multiply.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.o -c /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/app/test_matrix_multiply.cpp

CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/app/test_matrix_multiply.cpp > CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.i

CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/app/test_matrix_multiply.cpp -o CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.s

# Object files for target test_matrix_multiply
test_matrix_multiply_OBJECTS = \
"CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.o"

# External object files for target test_matrix_multiply
test_matrix_multiply_EXTERNAL_OBJECTS =

test_matrix_multiply: CMakeFiles/test_matrix_multiply.dir/app/test_matrix_multiply.cpp.o
test_matrix_multiply: CMakeFiles/test_matrix_multiply.dir/build.make
test_matrix_multiply: CMakeFiles/test_matrix_multiply.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_matrix_multiply"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_matrix_multiply.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_matrix_multiply.dir/build: test_matrix_multiply

.PHONY : CMakeFiles/test_matrix_multiply.dir/build

CMakeFiles/test_matrix_multiply.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_matrix_multiply.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_matrix_multiply.dir/clean

CMakeFiles/test_matrix_multiply.dir/depend:
	cd /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/bin /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/bin /Users/albert/Desktop/Programming/SENG475-Assignments/assignment5/matrix/bin/CMakeFiles/test_matrix_multiply.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_matrix_multiply.dir/depend
