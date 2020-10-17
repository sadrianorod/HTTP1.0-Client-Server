cmake_minimum_required(VERSION 3.10)
project(LAB2CES35)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11 -O2")

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)



add_executable(web-client webClient.cpp)

add_subdirectory("Resource")
add_executable(web-server webServer.cpp ${resources})
target_link_libraries(web-server Resource)
