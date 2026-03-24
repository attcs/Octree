cmake_minimum_required(VERSION 3.15)
project(OrthoTreeSerializationExternalTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(cereal CONFIG REQUIRED)
find_package(msgpack-cxx CONFIG REQUIRED)

add_executable(serialization_test main.cpp)

# OrthoTree is header-only, just point to the include dir
target_include_directories(serialization_test PRIVATE "../../include")

# Link libraries
target_link_libraries(serialization_test PRIVATE cereal::cereal msgpack-cxx)
