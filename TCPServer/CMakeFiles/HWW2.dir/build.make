# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer

# Include any dependencies generated for this target.
include CMakeFiles/HWW2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HWW2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HWW2.dir/flags.make

CMakeFiles/HWW2.dir/main.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/main.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/main.cpp

CMakeFiles/HWW2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/main.cpp > CMakeFiles/HWW2.dir/main.cpp.i

CMakeFiles/HWW2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/main.cpp -o CMakeFiles/HWW2.dir/main.cpp.s

CMakeFiles/HWW2.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/main.cpp.o.requires

CMakeFiles/HWW2.dir/main.cpp.o.provides: CMakeFiles/HWW2.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/main.cpp.o.provides

CMakeFiles/HWW2.dir/main.cpp.o.provides.build: CMakeFiles/HWW2.dir/main.cpp.o

CMakeFiles/HWW2.dir/MyEpoll.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/MyEpoll.cpp.o: MyEpoll.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/MyEpoll.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/MyEpoll.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/MyEpoll.cpp

CMakeFiles/HWW2.dir/MyEpoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/MyEpoll.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/MyEpoll.cpp > CMakeFiles/HWW2.dir/MyEpoll.cpp.i

CMakeFiles/HWW2.dir/MyEpoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/MyEpoll.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/MyEpoll.cpp -o CMakeFiles/HWW2.dir/MyEpoll.cpp.s

CMakeFiles/HWW2.dir/MyEpoll.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/MyEpoll.cpp.o.requires

CMakeFiles/HWW2.dir/MyEpoll.cpp.o.provides: CMakeFiles/HWW2.dir/MyEpoll.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/MyEpoll.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/MyEpoll.cpp.o.provides

CMakeFiles/HWW2.dir/MyEpoll.cpp.o.provides.build: CMakeFiles/HWW2.dir/MyEpoll.cpp.o

CMakeFiles/HWW2.dir/MyClient.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/MyClient.cpp.o: MyClient.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/MyClient.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/MyClient.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/MyClient.cpp

CMakeFiles/HWW2.dir/MyClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/MyClient.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/MyClient.cpp > CMakeFiles/HWW2.dir/MyClient.cpp.i

CMakeFiles/HWW2.dir/MyClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/MyClient.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/MyClient.cpp -o CMakeFiles/HWW2.dir/MyClient.cpp.s

CMakeFiles/HWW2.dir/MyClient.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/MyClient.cpp.o.requires

CMakeFiles/HWW2.dir/MyClient.cpp.o.provides: CMakeFiles/HWW2.dir/MyClient.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/MyClient.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/MyClient.cpp.o.provides

CMakeFiles/HWW2.dir/MyClient.cpp.o.provides.build: CMakeFiles/HWW2.dir/MyClient.cpp.o

CMakeFiles/HWW2.dir/HttpWorker.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/HttpWorker.cpp.o: HttpWorker.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/HttpWorker.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/HttpWorker.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/HttpWorker.cpp

CMakeFiles/HWW2.dir/HttpWorker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/HttpWorker.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/HttpWorker.cpp > CMakeFiles/HWW2.dir/HttpWorker.cpp.i

CMakeFiles/HWW2.dir/HttpWorker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/HttpWorker.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/HttpWorker.cpp -o CMakeFiles/HWW2.dir/HttpWorker.cpp.s

CMakeFiles/HWW2.dir/HttpWorker.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/HttpWorker.cpp.o.requires

CMakeFiles/HWW2.dir/HttpWorker.cpp.o.provides: CMakeFiles/HWW2.dir/HttpWorker.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/HttpWorker.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/HttpWorker.cpp.o.provides

CMakeFiles/HWW2.dir/HttpWorker.cpp.o.provides.build: CMakeFiles/HWW2.dir/HttpWorker.cpp.o

CMakeFiles/HWW2.dir/Message.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/Message.cpp.o: Message.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/Message.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/Message.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/Message.cpp

CMakeFiles/HWW2.dir/Message.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/Message.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/Message.cpp > CMakeFiles/HWW2.dir/Message.cpp.i

CMakeFiles/HWW2.dir/Message.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/Message.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/Message.cpp -o CMakeFiles/HWW2.dir/Message.cpp.s

CMakeFiles/HWW2.dir/Message.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/Message.cpp.o.requires

CMakeFiles/HWW2.dir/Message.cpp.o.provides: CMakeFiles/HWW2.dir/Message.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/Message.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/Message.cpp.o.provides

CMakeFiles/HWW2.dir/Message.cpp.o.provides.build: CMakeFiles/HWW2.dir/Message.cpp.o

CMakeFiles/HWW2.dir/Tools.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/Tools.cpp.o: Tools.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/Tools.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/Tools.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/Tools.cpp

CMakeFiles/HWW2.dir/Tools.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/Tools.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/Tools.cpp > CMakeFiles/HWW2.dir/Tools.cpp.i

CMakeFiles/HWW2.dir/Tools.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/Tools.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/Tools.cpp -o CMakeFiles/HWW2.dir/Tools.cpp.s

CMakeFiles/HWW2.dir/Tools.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/Tools.cpp.o.requires

CMakeFiles/HWW2.dir/Tools.cpp.o.provides: CMakeFiles/HWW2.dir/Tools.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/Tools.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/Tools.cpp.o.provides

CMakeFiles/HWW2.dir/Tools.cpp.o.provides.build: CMakeFiles/HWW2.dir/Tools.cpp.o

CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o: reversiServer/GameState.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/reversiServer/GameState.cpp

CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/reversiServer/GameState.cpp > CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.i

CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/reversiServer/GameState.cpp -o CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.s

CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.requires

CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.provides: CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.provides

CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.provides.build: CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o

CMakeFiles/HWW2.dir/debug.cpp.o: CMakeFiles/HWW2.dir/flags.make
CMakeFiles/HWW2.dir/debug.cpp.o: debug.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HWW2.dir/debug.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HWW2.dir/debug.cpp.o -c /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/debug.cpp

CMakeFiles/HWW2.dir/debug.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HWW2.dir/debug.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/debug.cpp > CMakeFiles/HWW2.dir/debug.cpp.i

CMakeFiles/HWW2.dir/debug.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HWW2.dir/debug.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/debug.cpp -o CMakeFiles/HWW2.dir/debug.cpp.s

CMakeFiles/HWW2.dir/debug.cpp.o.requires:
.PHONY : CMakeFiles/HWW2.dir/debug.cpp.o.requires

CMakeFiles/HWW2.dir/debug.cpp.o.provides: CMakeFiles/HWW2.dir/debug.cpp.o.requires
	$(MAKE) -f CMakeFiles/HWW2.dir/build.make CMakeFiles/HWW2.dir/debug.cpp.o.provides.build
.PHONY : CMakeFiles/HWW2.dir/debug.cpp.o.provides

CMakeFiles/HWW2.dir/debug.cpp.o.provides.build: CMakeFiles/HWW2.dir/debug.cpp.o

# Object files for target HWW2
HWW2_OBJECTS = \
"CMakeFiles/HWW2.dir/main.cpp.o" \
"CMakeFiles/HWW2.dir/MyEpoll.cpp.o" \
"CMakeFiles/HWW2.dir/MyClient.cpp.o" \
"CMakeFiles/HWW2.dir/HttpWorker.cpp.o" \
"CMakeFiles/HWW2.dir/Message.cpp.o" \
"CMakeFiles/HWW2.dir/Tools.cpp.o" \
"CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o" \
"CMakeFiles/HWW2.dir/debug.cpp.o"

# External object files for target HWW2
HWW2_EXTERNAL_OBJECTS =

HWW2: CMakeFiles/HWW2.dir/main.cpp.o
HWW2: CMakeFiles/HWW2.dir/MyEpoll.cpp.o
HWW2: CMakeFiles/HWW2.dir/MyClient.cpp.o
HWW2: CMakeFiles/HWW2.dir/HttpWorker.cpp.o
HWW2: CMakeFiles/HWW2.dir/Message.cpp.o
HWW2: CMakeFiles/HWW2.dir/Tools.cpp.o
HWW2: CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o
HWW2: CMakeFiles/HWW2.dir/debug.cpp.o
HWW2: CMakeFiles/HWW2.dir/build.make
HWW2: CMakeFiles/HWW2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable HWW2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HWW2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HWW2.dir/build: HWW2
.PHONY : CMakeFiles/HWW2.dir/build

CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/main.cpp.o.requires
CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/MyEpoll.cpp.o.requires
CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/MyClient.cpp.o.requires
CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/HttpWorker.cpp.o.requires
CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/Message.cpp.o.requires
CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/Tools.cpp.o.requires
CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/reversiServer/GameState.cpp.o.requires
CMakeFiles/HWW2.dir/requires: CMakeFiles/HWW2.dir/debug.cpp.o.requires
.PHONY : CMakeFiles/HWW2.dir/requires

CMakeFiles/HWW2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HWW2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HWW2.dir/clean

CMakeFiles/HWW2.dir/depend:
	cd /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer /home/vanya/Documents/prog/C/homeWorkCpp/TCPServer/CMakeFiles/HWW2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HWW2.dir/depend

