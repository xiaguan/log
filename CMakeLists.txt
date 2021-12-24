cmake_minimum_required(VERSION 3.21)
project(logexample)


add_subdirectory(log)

add_executable(logexample main.cpp)

target_link_libraries(logexample Log)

