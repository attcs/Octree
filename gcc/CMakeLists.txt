cmake_minimum_required(VERSION 3.10)

project(octree)
find_package(TBB REQUIRED COMPONENTS tbb)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTBB_ENABLE_IPO=off -std=c++20")
include_directories(../)
add_executable(octree main.cpp)
target_link_libraries(octree tbb)