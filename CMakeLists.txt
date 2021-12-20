# 项目基础信息
cmake_minimum_required(VERSION 3.0)
project(LOGGER VERSION 1.0)
set(CMAKE_CXX_STANDARD 11)

# 包含linux-pthread库
set(CMAKE_BUILD_TYPE "Release")
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")
else( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -fopenmp")
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )

# 设置文件输出路径（库文件，可执行文件，包含路径）
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/include/)

# 编译出动态链接库
add_library(ThreadPool SHARED
        src/ThreadPool.cpp
        
)
add_library(Log SHARED
            src/Log.cpp
        
)
# 编译出可执行文件
add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)

# 运行时查找动态链接库
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath=./lib:./:../lib")

# 目标程序链接关系
target_link_libraries(main 
                      ThreadPool
                      Log
                      )
