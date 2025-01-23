cmake_minimum_required(VERSION 3.10)

project(benchmarks)
find_package(TBB QUIET)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "Download Google Benchmark dependencies")

option(ENABLE_OPTIMIZATIONS "Enable high-performance compiler optimizations" ON)

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

if (ENABLE_OPTIMIZATIONS)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(benchmarks PRIVATE -DTBB_ENABLE_IPO=off -march=native -mavx2 -O3 -ffast-math)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(benchmarks PRIVATE /O2 /fp:fast)
    endif()
endif()
