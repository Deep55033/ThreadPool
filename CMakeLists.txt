cmake_minimum_required(VERSION 3.21)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ThreadPool
        prthread.cpp
        prthread.h
        threadpool.cpp
        threadpool.h server_socket.hpp main.cpp task.h)

target_link_libraries(ThreadPool event)