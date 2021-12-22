cmake_minimum_required(VERSION 3.21)
project(log)

set(CMAKE_CXX_STANDARD 14)

include_directories(log)

add_executable(log
        log/log.cpp
        log/log.h
        log/main.cpp)
