cmake_minimum_required(VERSION 3.10)

project(octree)
find_package(TBB QUIET)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTBB_ENABLE_IPO=off -std=c++20")
include_directories(../)
add_executable(octree main.cpp)

if (TBB_FOUND)
  target_link_libraries(octree TBB::tbb)
  message(STATUS "TBB library is linked.")
else()
  message(STATUS "TBB library is not linked.")
endif()