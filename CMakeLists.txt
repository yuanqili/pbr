cmake_minimum_required(VERSION 3.6)
project(pbr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp
        shape/Sphere.cpp
        shape/Sphere.h
        core/pbr.cpp
        core/pbr.h
        core/Point.cpp
        core/Point.h
        core/Vector.cpp
        core/Vector.h
        core/Transform.cpp
        core/Transform.h)
add_executable(pbr ${SOURCE_FILES})
