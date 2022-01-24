# 单独测试test

cmake_minimum_required(VERSION 3.20)
project(test)

add_subdirectory(../lib)

add_executable(serv serv.cpp)
add_executable(client client.cpp)

target_link_libraries(serv Su)
target_link_libraries(client Su)
