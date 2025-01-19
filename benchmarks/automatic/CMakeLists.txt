cmake_minimum_required(VERSION 3.10)

project(benchmarks)
find_package(TBB QUIET)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTBB_ENABLE_IPO=off -std=c++20")
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "Download Google Benchmark dependencies")

include_directories(../)

# Add Google Benchmark as a subdirectory
add_subdirectory(external/benchmark)

# Link the benchmark library to your targets
add_executable(benchmarks main.cpp)

target_link_libraries(benchmarks benchmark::benchmark)

if (TBB_FOUND)
  target_link_libraries(benchmarks TBB::tbb)
  message(STATUS "TBB library is linked.")
else()
  message(STATUS "TBB library is not linked.")
endif()