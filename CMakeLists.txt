cmake_minimum_required(VERSION 3.21)

project(log)

include_directories(lib)

aux_source_directory(. SRC_LIST)
aux_source_directory(lib SRC_LIST)

add_executable(log ${SRC_LIST} lib/mysock.h lib/mysock.cpp)