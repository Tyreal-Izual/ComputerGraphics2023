# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /home/frederickzou/.local/share/JetBrains/Toolbox/apps/clion/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/frederickzou/.local/share/JetBrains/Toolbox/apps/clion/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/RedNoise.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RedNoise.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RedNoise.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RedNoise.dir/flags.make

CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasPoint.cpp
CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasPoint.cpp

CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasPoint.cpp > CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasPoint.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasTriangle.cpp
CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasTriangle.cpp

CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasTriangle.cpp > CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/CanvasTriangle.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Colour.cpp
CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Colour.cpp

CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Colour.cpp > CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Colour.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/DrawingWindow.cpp
CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/DrawingWindow.cpp

CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/DrawingWindow.cpp > CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/DrawingWindow.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/ModelTriangle.cpp
CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/ModelTriangle.cpp

CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/ModelTriangle.cpp > CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/ModelTriangle.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/RayTriangleIntersection.cpp
CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/RayTriangleIntersection.cpp

CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/RayTriangleIntersection.cpp > CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/RayTriangleIntersection.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TextureMap.cpp
CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TextureMap.cpp

CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TextureMap.cpp > CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TextureMap.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TexturePoint.cpp
CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TexturePoint.cpp

CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TexturePoint.cpp > CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/TexturePoint.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.s

CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Utils.cpp
CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o -MF CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o.d -o CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Utils.cpp

CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Utils.cpp > CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.i

CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/libs/sdw/Utils.cpp -o CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.s

CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o: CMakeFiles/RedNoise.dir/flags.make
CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o: /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/src/RedNoise.cpp
CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o: CMakeFiles/RedNoise.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o -MF CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o.d -o CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o -c /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/src/RedNoise.cpp

CMakeFiles/RedNoise.dir/src/RedNoise.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RedNoise.dir/src/RedNoise.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/src/RedNoise.cpp > CMakeFiles/RedNoise.dir/src/RedNoise.cpp.i

CMakeFiles/RedNoise.dir/src/RedNoise.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RedNoise.dir/src/RedNoise.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/src/RedNoise.cpp -o CMakeFiles/RedNoise.dir/src/RedNoise.cpp.s

# Object files for target RedNoise
RedNoise_OBJECTS = \
"CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o" \
"CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o" \
"CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o"

# External object files for target RedNoise
RedNoise_EXTERNAL_OBJECTS =

RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/CanvasPoint.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/CanvasTriangle.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/Colour.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/DrawingWindow.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/ModelTriangle.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/RayTriangleIntersection.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/TextureMap.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/TexturePoint.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/libs/sdw/Utils.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/src/RedNoise.cpp.o
RedNoise: CMakeFiles/RedNoise.dir/build.make
RedNoise: /usr/local/lib/libSDL2.so
RedNoise: CMakeFiles/RedNoise.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable RedNoise"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RedNoise.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RedNoise.dir/build: RedNoise
.PHONY : CMakeFiles/RedNoise.dir/build

CMakeFiles/RedNoise.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RedNoise.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RedNoise.dir/clean

CMakeFiles/RedNoise.dir/depend:
	cd /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug /home/frederickzou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cmake-build-debug/CMakeFiles/RedNoise.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RedNoise.dir/depend

