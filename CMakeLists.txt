cmake_minimum_required(VERSION 3.21)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        include_directories(include)
        link_directories(lib)
endif()

add_executable(ThreadPool
        src/prthread.cpp
        src/prthread.h
        src/threadpool.cpp
        src/ThreadPool.h src/server_socket.hpp src/main.cpp src/task.h)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")  
        target_link_libraries(ThreadPool libeventwin Iphlpapi.lib ws2_32.lib)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_libraries(ThreadPool event)
endif()