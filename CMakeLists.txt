cmake_minimum_required(VERSION 3.6)
project(pbr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(pbr ${SOURCE_FILES})