# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.22.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.22.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tom/Documents/clifford-errors

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tom/Documents/clifford-errors/src

# Include any dependencies generated for this target.
include src/CMakeFiles/czErrors.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/czErrors.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/czErrors.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/czErrors.dir/flags.make

src/CMakeFiles/czErrors.dir/main.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/main.cpp.o: main.cpp
src/CMakeFiles/czErrors.dir/main.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/czErrors.dir/main.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/main.cpp.o -MF CMakeFiles/czErrors.dir/main.cpp.o.d -o CMakeFiles/czErrors.dir/main.cpp.o -c /Users/tom/Documents/clifford-errors/src/main.cpp

src/CMakeFiles/czErrors.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/main.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/main.cpp > CMakeFiles/czErrors.dir/main.cpp.i

src/CMakeFiles/czErrors.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/main.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/main.cpp -o CMakeFiles/czErrors.dir/main.cpp.s

src/CMakeFiles/czErrors.dir/czErrorGen.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/czErrorGen.cpp.o: czErrorGen.cpp
src/CMakeFiles/czErrors.dir/czErrorGen.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/czErrors.dir/czErrorGen.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/czErrorGen.cpp.o -MF CMakeFiles/czErrors.dir/czErrorGen.cpp.o.d -o CMakeFiles/czErrors.dir/czErrorGen.cpp.o -c /Users/tom/Documents/clifford-errors/src/czErrorGen.cpp

src/CMakeFiles/czErrors.dir/czErrorGen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/czErrorGen.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/czErrorGen.cpp > CMakeFiles/czErrors.dir/czErrorGen.cpp.i

src/CMakeFiles/czErrors.dir/czErrorGen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/czErrorGen.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/czErrorGen.cpp -o CMakeFiles/czErrors.dir/czErrorGen.cpp.s

src/CMakeFiles/czErrors.dir/decoderCubic.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/decoderCubic.cpp.o: decoderCubic.cpp
src/CMakeFiles/czErrors.dir/decoderCubic.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/czErrors.dir/decoderCubic.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/decoderCubic.cpp.o -MF CMakeFiles/czErrors.dir/decoderCubic.cpp.o.d -o CMakeFiles/czErrors.dir/decoderCubic.cpp.o -c /Users/tom/Documents/clifford-errors/src/decoderCubic.cpp

src/CMakeFiles/czErrors.dir/decoderCubic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/decoderCubic.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/decoderCubic.cpp > CMakeFiles/czErrors.dir/decoderCubic.cpp.i

src/CMakeFiles/czErrors.dir/decoderCubic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/decoderCubic.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/decoderCubic.cpp -o CMakeFiles/czErrors.dir/decoderCubic.cpp.s

src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o: decoderRhombic1.cpp
src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o -MF CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o.d -o CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o -c /Users/tom/Documents/clifford-errors/src/decoderRhombic1.cpp

src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/decoderRhombic1.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/decoderRhombic1.cpp > CMakeFiles/czErrors.dir/decoderRhombic1.cpp.i

src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/decoderRhombic1.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/decoderRhombic1.cpp -o CMakeFiles/czErrors.dir/decoderRhombic1.cpp.s

src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o: decoderRhombic2.cpp
src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o -MF CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o.d -o CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o -c /Users/tom/Documents/clifford-errors/src/decoderRhombic2.cpp

src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/decoderRhombic2.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/decoderRhombic2.cpp > CMakeFiles/czErrors.dir/decoderRhombic2.cpp.i

src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/decoderRhombic2.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/decoderRhombic2.cpp -o CMakeFiles/czErrors.dir/decoderRhombic2.cpp.s

src/CMakeFiles/czErrors.dir/cubic.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/cubic.cpp.o: cubic.cpp
src/CMakeFiles/czErrors.dir/cubic.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/czErrors.dir/cubic.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/cubic.cpp.o -MF CMakeFiles/czErrors.dir/cubic.cpp.o.d -o CMakeFiles/czErrors.dir/cubic.cpp.o -c /Users/tom/Documents/clifford-errors/src/cubic.cpp

src/CMakeFiles/czErrors.dir/cubic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/cubic.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/cubic.cpp > CMakeFiles/czErrors.dir/cubic.cpp.i

src/CMakeFiles/czErrors.dir/cubic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/cubic.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/cubic.cpp -o CMakeFiles/czErrors.dir/cubic.cpp.s

src/CMakeFiles/czErrors.dir/rhombic1.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/rhombic1.cpp.o: rhombic1.cpp
src/CMakeFiles/czErrors.dir/rhombic1.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/czErrors.dir/rhombic1.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/rhombic1.cpp.o -MF CMakeFiles/czErrors.dir/rhombic1.cpp.o.d -o CMakeFiles/czErrors.dir/rhombic1.cpp.o -c /Users/tom/Documents/clifford-errors/src/rhombic1.cpp

src/CMakeFiles/czErrors.dir/rhombic1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/rhombic1.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/rhombic1.cpp > CMakeFiles/czErrors.dir/rhombic1.cpp.i

src/CMakeFiles/czErrors.dir/rhombic1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/rhombic1.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/rhombic1.cpp -o CMakeFiles/czErrors.dir/rhombic1.cpp.s

src/CMakeFiles/czErrors.dir/rhombic2.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/rhombic2.cpp.o: rhombic2.cpp
src/CMakeFiles/czErrors.dir/rhombic2.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/czErrors.dir/rhombic2.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/rhombic2.cpp.o -MF CMakeFiles/czErrors.dir/rhombic2.cpp.o.d -o CMakeFiles/czErrors.dir/rhombic2.cpp.o -c /Users/tom/Documents/clifford-errors/src/rhombic2.cpp

src/CMakeFiles/czErrors.dir/rhombic2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/rhombic2.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/rhombic2.cpp > CMakeFiles/czErrors.dir/rhombic2.cpp.i

src/CMakeFiles/czErrors.dir/rhombic2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/rhombic2.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/rhombic2.cpp -o CMakeFiles/czErrors.dir/rhombic2.cpp.s

src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o: rhombicGeneric.cpp
src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o -MF CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o.d -o CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o -c /Users/tom/Documents/clifford-errors/src/rhombicGeneric.cpp

src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/rhombicGeneric.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/rhombicGeneric.cpp > CMakeFiles/czErrors.dir/rhombicGeneric.cpp.i

src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/rhombicGeneric.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/rhombicGeneric.cpp -o CMakeFiles/czErrors.dir/rhombicGeneric.cpp.s

src/CMakeFiles/czErrors.dir/lattice.cpp.o: src/CMakeFiles/czErrors.dir/flags.make
src/CMakeFiles/czErrors.dir/lattice.cpp.o: lattice.cpp
src/CMakeFiles/czErrors.dir/lattice.cpp.o: src/CMakeFiles/czErrors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/CMakeFiles/czErrors.dir/lattice.cpp.o"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/czErrors.dir/lattice.cpp.o -MF CMakeFiles/czErrors.dir/lattice.cpp.o.d -o CMakeFiles/czErrors.dir/lattice.cpp.o -c /Users/tom/Documents/clifford-errors/src/lattice.cpp

src/CMakeFiles/czErrors.dir/lattice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/czErrors.dir/lattice.cpp.i"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tom/Documents/clifford-errors/src/lattice.cpp > CMakeFiles/czErrors.dir/lattice.cpp.i

src/CMakeFiles/czErrors.dir/lattice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/czErrors.dir/lattice.cpp.s"
	cd /Users/tom/Documents/clifford-errors/src/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tom/Documents/clifford-errors/src/lattice.cpp -o CMakeFiles/czErrors.dir/lattice.cpp.s

# Object files for target czErrors
czErrors_OBJECTS = \
"CMakeFiles/czErrors.dir/main.cpp.o" \
"CMakeFiles/czErrors.dir/czErrorGen.cpp.o" \
"CMakeFiles/czErrors.dir/decoderCubic.cpp.o" \
"CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o" \
"CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o" \
"CMakeFiles/czErrors.dir/cubic.cpp.o" \
"CMakeFiles/czErrors.dir/rhombic1.cpp.o" \
"CMakeFiles/czErrors.dir/rhombic2.cpp.o" \
"CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o" \
"CMakeFiles/czErrors.dir/lattice.cpp.o"

# External object files for target czErrors
czErrors_EXTERNAL_OBJECTS =

src/czErrors: src/CMakeFiles/czErrors.dir/main.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/czErrorGen.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/decoderCubic.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/decoderRhombic1.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/decoderRhombic2.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/cubic.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/rhombic1.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/rhombic2.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/rhombicGeneric.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/lattice.cpp.o
src/czErrors: src/CMakeFiles/czErrors.dir/build.make
src/czErrors: lib/blossom5/libPerfectMatching.a
src/czErrors: src/CMakeFiles/czErrors.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/tom/Documents/clifford-errors/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable czErrors"
	cd /Users/tom/Documents/clifford-errors/src/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/czErrors.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/czErrors.dir/build: src/czErrors
.PHONY : src/CMakeFiles/czErrors.dir/build

src/CMakeFiles/czErrors.dir/clean:
	cd /Users/tom/Documents/clifford-errors/src/src && $(CMAKE_COMMAND) -P CMakeFiles/czErrors.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/czErrors.dir/clean

src/CMakeFiles/czErrors.dir/depend:
	cd /Users/tom/Documents/clifford-errors/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tom/Documents/clifford-errors /Users/tom/Documents/clifford-errors/src /Users/tom/Documents/clifford-errors/src /Users/tom/Documents/clifford-errors/src/src /Users/tom/Documents/clifford-errors/src/src/CMakeFiles/czErrors.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/czErrors.dir/depend

