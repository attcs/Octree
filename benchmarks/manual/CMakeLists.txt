cmake_minimum_required(VERSION 3.10)

project(benchmarks)

find_package(TBB REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/include/tbb/")
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -march=native -mavx2 -O3 -ffast-math -fopt-info-vec -Wall")
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(../)
add_executable(${PROJECT_NAME} benchmarks.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC TBB::tbb)
#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)


