cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS -v)

set(CMAKE_BUILD_TYPE Release)

project(lib_test)

# 不同平台生成不同库
if(UNIX)
    aux_source_directory(${PROJECT_SOURCE_DIR}/lib LIB_SRCS)
    aux_source_directory(${PROJECT_SOURCE_DIR}/lib/TinyStl LIB_SRCS)
    aux_source_directory(${PROJECT_SOURCE_DIR}/lib/util LIB_SRCS)
    add_library(su OBJECT ${LIB_SRCS})
elseif(WIN32)
    aux_source_directory(${PROJECT_SOURCE_DIR}/lib/TinyStl LIB_SRCS)
    aux_source_directory(${PROJECT_SOURCE_DIR}/lib/util LIB_SRCS)
    add_library(su OBJECT ${PROJECT_SOURCE_DIR}/lib/log.cpp ${PROJECT_SOURCE_DIR}/lib/log.h
            ${LIB_SRCS})
endif()

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(yaml-cpp REQUIRED)

# 生成可执行文件，不同系统，不同的测试
if(UNIX)
    add_executable(test ${PROJECT_SOURCE_DIR}/test/test.cpp)
    target_link_libraries(test PUBLIC su yaml-cpp)
elseif(WIN32)
    add_executable(test ${PROJECT_SOURCE_DIR}/test/wintest.cpp)
    target_link_libraries(test PUBLIC su)
endif()


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
