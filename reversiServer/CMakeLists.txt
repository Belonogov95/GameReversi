cmake_minimum_required(VERSION 3.2)
project(reversiServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(reversiServer ${SOURCE_FILES} clients/ClientInterface.cpp clients/ClientInterface.h GameState.cpp
GameState.h GameServer.cpp GameServer.h clients/ClientConsole.cpp clients/ClientConsole.h debug.h)