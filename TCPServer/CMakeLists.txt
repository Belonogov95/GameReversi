cmake_minimum_required(VERSION 3.2)
project(HWW2)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GLIBCXX_DEBUG -fsanitize=address -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

set(SOURCE_FILES main.cpp)
add_executable(HWW2 ${SOURCE_FILES}
HttpWorker.cpp
Tools.cpp  reversiServer/GameState.cpp Executor.cpp TcpSocketServer.cpp  TcpSocketClient.cpp Message.cpp SmartSocket.cpp
FileDescriptor.cpp)


#add_executable(HWW2 ${SOURCE_FILES} MyEpoll.cpp MyEpoll.h MyClient.cpp MyClient.h debug.h
#HttpWorker.cpp HttpWorker.h Message.cpp Message.h Tools.cpp Tools.h reversiServer/GameState.cpp reversiServer/GameState.h debug.cpp debug.h Executor.cpp Executor.h TcpSocketServer.cpp TcpSocketServer.h)


