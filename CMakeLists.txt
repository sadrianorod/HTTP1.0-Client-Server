
project(LAB2CES35)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11 -O2")

add_executable(echo-client echo-client.cpp)
add_executable(echo-server echo-server.cpp)