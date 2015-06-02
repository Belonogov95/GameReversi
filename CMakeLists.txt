cmake_minimum_required(VERSION 3.2)
project(HWW2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(HWW2 ${SOURCE_FILES} MyEpoll.cpp MyEpoll.h MySocket.cpp MySocket.h MyClient.cpp MyClient.h)