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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2024.2.2\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2024.2.2\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/lab1_window.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lab1_window.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lab1_window.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab1_window.dir/flags.make

CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj: CMakeFiles/lab1_window.dir/flags.make
CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj: CMakeFiles/lab1_window.dir/includes_CXX.rsp
CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj: C:/Users/sandr/Desktop/College/Computer\ Graphics/Graphics-Final-Project/ModernCelt/project/lab1_window.cpp
CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj: CMakeFiles/lab1_window.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj -MF CMakeFiles\lab1_window.dir\project\lab1_window.cpp.obj.d -o CMakeFiles\lab1_window.dir\project\lab1_window.cpp.obj -c "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\project\lab1_window.cpp"

CMakeFiles/lab1_window.dir/project/lab1_window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lab1_window.dir/project/lab1_window.cpp.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\project\lab1_window.cpp" > CMakeFiles\lab1_window.dir\project\lab1_window.cpp.i

CMakeFiles/lab1_window.dir/project/lab1_window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lab1_window.dir/project/lab1_window.cpp.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\project\lab1_window.cpp" -o CMakeFiles\lab1_window.dir\project\lab1_window.cpp.s

# Object files for target lab1_window
lab1_window_OBJECTS = \
"CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj"

# External object files for target lab1_window
lab1_window_EXTERNAL_OBJECTS =

lab1_window.exe: CMakeFiles/lab1_window.dir/project/lab1_window.cpp.obj
lab1_window.exe: CMakeFiles/lab1_window.dir/build.make
lab1_window.exe: glfw3dll.a
lab1_window.exe: CMakeFiles/lab1_window.dir/linkLibs.rsp
lab1_window.exe: CMakeFiles/lab1_window.dir/objects1.rsp
lab1_window.exe: CMakeFiles/lab1_window.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lab1_window.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lab1_window.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab1_window.dir/build: lab1_window.exe
.PHONY : CMakeFiles/lab1_window.dir/build

CMakeFiles/lab1_window.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab1_window.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lab1_window.dir/clean

CMakeFiles/lab1_window.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt" "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt" "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\cmake-build-debug" "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\cmake-build-debug" "C:\Users\sandr\Desktop\College\Computer Graphics\Graphics-Final-Project\ModernCelt\cmake-build-debug\CMakeFiles\lab1_window.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/lab1_window.dir/depend

