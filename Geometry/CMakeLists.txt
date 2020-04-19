cmake_minimum_required(VERSION 3.15)
project(Geometry)

set(CMAKE_CXX_STANDARD 14)

add_executable(Geometry main.cpp main.h)

set(CMAKE_CXX_FLAGS "-Wall -Werror")