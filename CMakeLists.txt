cmake_minimum_required(VERSION 3.10)
project(LAB2CES35)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11 -O2")

add_executable(socketClient socketclient.cpp)
add_executable(socketServer socketserver.cpp)

add_subdirectory("Resource")
add_executable(webServer webServer.cpp ${resources})
target_link_libraries(webServer Resource)