cmake_minimum_required(VERSION 3.15)
project(OrthoTreeCerealTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find cereal
find_package(cereal CONFIG REQUIRED)

add_executable(cereal_test main.cpp)

# OrthoTree is header-only, just point to the include dir
target_include_directories(cereal_test PRIVATE "../../include")

# Link cereal
target_link_libraries(cereal_test PRIVATE cereal::cereal)
