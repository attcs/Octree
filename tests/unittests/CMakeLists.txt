cmake_minimum_required(VERSION 3.14)
project(OrthoTreeUnitTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Find optional TBB (needed for parallel execution validation)
find_package(TBB QUIET)

# Find Boost (required for boost adapter tests)
find_package(Boost REQUIRED)

# Find Eigen3
find_package(Eigen3 CONFIG REQUIRED)

# Define unit tests target
add_executable(orthotree_unittests
  adapter.tests.cpp
  bitset.arithmetic.tests.cpp
  bvh.tests.cpp
  collision.tests.cpp
  compile.tests.cpp
  example.tests.cpp
  general.tests.cpp
  knn.tests.cpp
  longint.tests.cpp
  mr.tests.cpp
  performance.tests.cpp
  random.tests.cpp
  search.tests.cpp
  serialize.tests.cpp
  single_header.tests.cpp
  tree_core.tests.cpp
)

# Target configuration
target_include_directories(orthotree_unittests PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(orthotree_unittests PRIVATE
  gtest
  gtest_main
  Boost::boost
  Eigen3::Eigen
)

if (TBB_FOUND)
  target_link_libraries(orthotree_unittests PRIVATE TBB::tbb)
  target_compile_definitions(orthotree_unittests PRIVATE TBB_AVAILABLE)
endif()

# Enable CTest runner discovery
include(GoogleTest)
gtest_discover_tests(orthotree_unittests)
