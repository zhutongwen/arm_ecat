# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xw/am437/nfs/arm-ecat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xw/am437/nfs/arm-ecat/build

# Include any dependencies generated for this target.
include src/socket/CMakeFiles/UDPclint.dir/depend.make

# Include the progress variables for this target.
include src/socket/CMakeFiles/UDPclint.dir/progress.make

# Include the compile flags for this target's objects.
include src/socket/CMakeFiles/UDPclint.dir/flags.make

src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o: src/socket/CMakeFiles/UDPclint.dir/flags.make
src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o: ../src/socket/UDPclientWithConnect.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xw/am437/nfs/arm-ecat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o"
	cd /home/xw/am437/nfs/arm-ecat/build/src/socket && /usr/bin/arm-linux-gnueabihf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o -c /home/xw/am437/nfs/arm-ecat/src/socket/UDPclientWithConnect.cpp

src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.i"
	cd /home/xw/am437/nfs/arm-ecat/build/src/socket && /usr/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xw/am437/nfs/arm-ecat/src/socket/UDPclientWithConnect.cpp > CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.i

src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.s"
	cd /home/xw/am437/nfs/arm-ecat/build/src/socket && /usr/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xw/am437/nfs/arm-ecat/src/socket/UDPclientWithConnect.cpp -o CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.s

src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.requires:

.PHONY : src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.requires

src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.provides: src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.requires
	$(MAKE) -f src/socket/CMakeFiles/UDPclint.dir/build.make src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.provides.build
.PHONY : src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.provides

src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.provides.build: src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o


# Object files for target UDPclint
UDPclint_OBJECTS = \
"CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o"

# External object files for target UDPclint
UDPclint_EXTERNAL_OBJECTS =

bin/UDPclint: src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o
bin/UDPclint: src/socket/CMakeFiles/UDPclint.dir/build.make
bin/UDPclint: src/socket/CMakeFiles/UDPclint.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xw/am437/nfs/arm-ecat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/UDPclint"
	cd /home/xw/am437/nfs/arm-ecat/build/src/socket && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UDPclint.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/socket/CMakeFiles/UDPclint.dir/build: bin/UDPclint

.PHONY : src/socket/CMakeFiles/UDPclint.dir/build

src/socket/CMakeFiles/UDPclint.dir/requires: src/socket/CMakeFiles/UDPclint.dir/UDPclientWithConnect.cpp.o.requires

.PHONY : src/socket/CMakeFiles/UDPclint.dir/requires

src/socket/CMakeFiles/UDPclint.dir/clean:
	cd /home/xw/am437/nfs/arm-ecat/build/src/socket && $(CMAKE_COMMAND) -P CMakeFiles/UDPclint.dir/cmake_clean.cmake
.PHONY : src/socket/CMakeFiles/UDPclint.dir/clean

src/socket/CMakeFiles/UDPclint.dir/depend:
	cd /home/xw/am437/nfs/arm-ecat/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xw/am437/nfs/arm-ecat /home/xw/am437/nfs/arm-ecat/src/socket /home/xw/am437/nfs/arm-ecat/build /home/xw/am437/nfs/arm-ecat/build/src/socket /home/xw/am437/nfs/arm-ecat/build/src/socket/CMakeFiles/UDPclint.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/socket/CMakeFiles/UDPclint.dir/depend
