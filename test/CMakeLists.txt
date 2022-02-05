# 单独测试test
add_executable(client client.cpp)
add_executable(server serv.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(client PUBLIC Su Threads::Threads)
target_link_libraries(server PUBLIC Su Threads::Threads)

target_include_directories(client PUBLIC "${PROJECT_SOURCE_DIR}/lib")
target_include_directories(server PUBLIC "${PROJECT_SOURCE_DIR}/lib")
