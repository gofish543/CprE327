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
CMAKE_COMMAND = /opt/Clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/Clion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /srv/CprE327

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /srv/CprE327/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CprE327.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CprE327.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CprE327.dir/flags.make

CMakeFiles/CprE327.dir/src/dungeon.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/dungeon.cpp.o: ../src/dungeon.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CprE327.dir/src/dungeon.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/dungeon.cpp.o -c /srv/CprE327/src/dungeon.cpp

CMakeFiles/CprE327.dir/src/dungeon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/dungeon.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/dungeon.cpp > CMakeFiles/CprE327.dir/src/dungeon.cpp.i

CMakeFiles/CprE327.dir/src/dungeon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/dungeon.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/dungeon.cpp -o CMakeFiles/CprE327.dir/src/dungeon.cpp.s

CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.o: ../src/escape_from_darkness.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.o -c /srv/CprE327/src/escape_from_darkness.cpp

CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/escape_from_darkness.cpp > CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.i

CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/escape_from_darkness.cpp -o CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.s

CMakeFiles/CprE327.dir/src/floor.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/floor.cpp.o: ../src/floor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/CprE327.dir/src/floor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/floor.cpp.o -c /srv/CprE327/src/floor.cpp

CMakeFiles/CprE327.dir/src/floor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/floor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/floor.cpp > CMakeFiles/CprE327.dir/src/floor.cpp.i

CMakeFiles/CprE327.dir/src/floor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/floor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/floor.cpp -o CMakeFiles/CprE327.dir/src/floor.cpp.s

CMakeFiles/CprE327.dir/src/output.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/output.cpp.o: ../src/output.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/CprE327.dir/src/output.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/output.cpp.o -c /srv/CprE327/src/output.cpp

CMakeFiles/CprE327.dir/src/output.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/output.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/output.cpp > CMakeFiles/CprE327.dir/src/output.cpp.i

CMakeFiles/CprE327.dir/src/output.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/output.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/output.cpp -o CMakeFiles/CprE327.dir/src/output.cpp.s

CMakeFiles/CprE327.dir/src/resource.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/resource.cpp.o: ../src/resource.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/CprE327.dir/src/resource.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/resource.cpp.o -c /srv/CprE327/src/resource.cpp

CMakeFiles/CprE327.dir/src/resource.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/resource.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/resource.cpp > CMakeFiles/CprE327.dir/src/resource.cpp.i

CMakeFiles/CprE327.dir/src/resource.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/resource.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/resource.cpp -o CMakeFiles/CprE327.dir/src/resource.cpp.s

CMakeFiles/CprE327.dir/src/settings.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/settings.cpp.o: ../src/settings.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/CprE327.dir/src/settings.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/settings.cpp.o -c /srv/CprE327/src/settings.cpp

CMakeFiles/CprE327.dir/src/settings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/settings.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/settings.cpp > CMakeFiles/CprE327.dir/src/settings.cpp.i

CMakeFiles/CprE327.dir/src/settings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/settings.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/settings.cpp -o CMakeFiles/CprE327.dir/src/settings.cpp.s

CMakeFiles/CprE327.dir/src/terrains/border.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/terrains/border.cpp.o: ../src/terrains/border.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/CprE327.dir/src/terrains/border.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/terrains/border.cpp.o -c /srv/CprE327/src/terrains/border.cpp

CMakeFiles/CprE327.dir/src/terrains/border.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/terrains/border.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/terrains/border.cpp > CMakeFiles/CprE327.dir/src/terrains/border.cpp.i

CMakeFiles/CprE327.dir/src/terrains/border.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/terrains/border.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/terrains/border.cpp -o CMakeFiles/CprE327.dir/src/terrains/border.cpp.s

CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.o: ../src/terrains/corridor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.o -c /srv/CprE327/src/terrains/corridor.cpp

CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/terrains/corridor.cpp > CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.i

CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/terrains/corridor.cpp -o CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.s

CMakeFiles/CprE327.dir/src/terrains/rock.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/terrains/rock.cpp.o: ../src/terrains/rock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/CprE327.dir/src/terrains/rock.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/terrains/rock.cpp.o -c /srv/CprE327/src/terrains/rock.cpp

CMakeFiles/CprE327.dir/src/terrains/rock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/terrains/rock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/terrains/rock.cpp > CMakeFiles/CprE327.dir/src/terrains/rock.cpp.i

CMakeFiles/CprE327.dir/src/terrains/rock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/terrains/rock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/terrains/rock.cpp -o CMakeFiles/CprE327.dir/src/terrains/rock.cpp.s

CMakeFiles/CprE327.dir/src/terrains/room.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/terrains/room.cpp.o: ../src/terrains/room.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/CprE327.dir/src/terrains/room.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/terrains/room.cpp.o -c /srv/CprE327/src/terrains/room.cpp

CMakeFiles/CprE327.dir/src/terrains/room.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/terrains/room.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/terrains/room.cpp > CMakeFiles/CprE327.dir/src/terrains/room.cpp.i

CMakeFiles/CprE327.dir/src/terrains/room.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/terrains/room.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/terrains/room.cpp -o CMakeFiles/CprE327.dir/src/terrains/room.cpp.s

CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.o: ../src/terrains/staircase.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.o -c /srv/CprE327/src/terrains/staircase.cpp

CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/terrains/staircase.cpp > CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.i

CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/terrains/staircase.cpp -o CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.s

CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.o: ../src/terrains/terrain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.o -c /srv/CprE327/src/terrains/terrain.cpp

CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/terrains/terrain.cpp > CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.i

CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/terrains/terrain.cpp -o CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.s

CMakeFiles/CprE327.dir/src/vendor/queue.cpp.o: CMakeFiles/CprE327.dir/flags.make
CMakeFiles/CprE327.dir/src/vendor/queue.cpp.o: ../src/vendor/queue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/CprE327.dir/src/vendor/queue.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CprE327.dir/src/vendor/queue.cpp.o -c /srv/CprE327/src/vendor/queue.cpp

CMakeFiles/CprE327.dir/src/vendor/queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CprE327.dir/src/vendor/queue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /srv/CprE327/src/vendor/queue.cpp > CMakeFiles/CprE327.dir/src/vendor/queue.cpp.i

CMakeFiles/CprE327.dir/src/vendor/queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CprE327.dir/src/vendor/queue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /srv/CprE327/src/vendor/queue.cpp -o CMakeFiles/CprE327.dir/src/vendor/queue.cpp.s

# Object files for target CprE327
CprE327_OBJECTS = \
"CMakeFiles/CprE327.dir/src/dungeon.cpp.o" \
"CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.o" \
"CMakeFiles/CprE327.dir/src/floor.cpp.o" \
"CMakeFiles/CprE327.dir/src/output.cpp.o" \
"CMakeFiles/CprE327.dir/src/resource.cpp.o" \
"CMakeFiles/CprE327.dir/src/settings.cpp.o" \
"CMakeFiles/CprE327.dir/src/terrains/border.cpp.o" \
"CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.o" \
"CMakeFiles/CprE327.dir/src/terrains/rock.cpp.o" \
"CMakeFiles/CprE327.dir/src/terrains/room.cpp.o" \
"CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.o" \
"CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.o" \
"CMakeFiles/CprE327.dir/src/vendor/queue.cpp.o"

# External object files for target CprE327
CprE327_EXTERNAL_OBJECTS =

CprE327: CMakeFiles/CprE327.dir/src/dungeon.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/escape_from_darkness.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/floor.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/output.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/resource.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/settings.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/terrains/border.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/terrains/corridor.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/terrains/rock.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/terrains/room.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/terrains/staircase.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/terrains/terrain.cpp.o
CprE327: CMakeFiles/CprE327.dir/src/vendor/queue.cpp.o
CprE327: CMakeFiles/CprE327.dir/build.make
CprE327: CMakeFiles/CprE327.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/srv/CprE327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable CprE327"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CprE327.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CprE327.dir/build: CprE327

.PHONY : CMakeFiles/CprE327.dir/build

CMakeFiles/CprE327.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CprE327.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CprE327.dir/clean

CMakeFiles/CprE327.dir/depend:
	cd /srv/CprE327/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /srv/CprE327 /srv/CprE327 /srv/CprE327/cmake-build-debug /srv/CprE327/cmake-build-debug /srv/CprE327/cmake-build-debug/CMakeFiles/CprE327.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CprE327.dir/depend

