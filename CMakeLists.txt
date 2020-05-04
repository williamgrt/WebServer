cmake_minimum_required(VERSION 3.1)
project(WebServer)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_compile_options(-std=c++14)
add_compile_options(-g)

set(CXX_FLAGS
    -g
    -Wall
    -std=c++14
    -D_PTHREADSin
    -Wno-unused-parameter
)


set(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_subdirectory(src)
