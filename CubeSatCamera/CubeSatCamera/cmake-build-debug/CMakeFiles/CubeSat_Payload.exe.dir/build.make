# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CubeSat_Payload.exe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CubeSat_Payload.exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CubeSat_Payload.exe.dir/flags.make

CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.o: CMakeFiles/CubeSat_Payload.exe.dir/flags.make
CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.o -c /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/main.cpp

CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/main.cpp > CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.i

CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/main.cpp -o CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.s

# Object files for target CubeSat_Payload.exe
CubeSat_Payload_exe_OBJECTS = \
"CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.o"

# External object files for target CubeSat_Payload.exe
CubeSat_Payload_exe_EXTERNAL_OBJECTS =

CubeSat_Payload.exe: CMakeFiles/CubeSat_Payload.exe.dir/main.cpp.o
CubeSat_Payload.exe: CMakeFiles/CubeSat_Payload.exe.dir/build.make
CubeSat_Payload.exe: CMakeFiles/CubeSat_Payload.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CubeSat_Payload.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CubeSat_Payload.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CubeSat_Payload.exe.dir/build: CubeSat_Payload.exe

.PHONY : CMakeFiles/CubeSat_Payload.exe.dir/build

CMakeFiles/CubeSat_Payload.exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CubeSat_Payload.exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CubeSat_Payload.exe.dir/clean

CMakeFiles/CubeSat_Payload.exe.dir/depend:
	cd /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/cmake-build-debug /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/cmake-build-debug /Users/liamhebert/Documents/GitHub/CubeSat-Camera-Interface/CubeSatCamera/CubeSatCamera/cmake-build-debug/CMakeFiles/CubeSat_Payload.exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CubeSat_Payload.exe.dir/depend
