cmake_minimum_required(VERSION 3.10)

project(octree)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
include_directories(../)
add_executable(octree main.cpp)
