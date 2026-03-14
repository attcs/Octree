cmake_minimum_required(VERSION 3.14)

project(OrthoTree VERSION 3.1.0 LANGUAGES CXX)

# --- CPM.cmake (Optional but provided for convenience) ---
# If you want to use CPM to manage dependencies for tests or benchmarks
# include(cmake/CPM.cmake)

# --- Library Target ---
add_library(OrthoTree INTERFACE)
add_library(OrthoTree::OrthoTree ALIAS OrthoTree)

# C++20 requirement as per README
target_compile_features(OrthoTree INTERFACE cxx_std_20)

# Path to the headers
target_include_directories(OrthoTree INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# --- Installation ---
include(GNUInstallDirs)

# Install the headers
install(DIRECTORY include/orthotree DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Create and install the export set
install(TARGETS OrthoTree 
    EXPORT OrthoTreeTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT OrthoTreeTargets
    FILE OrthoTreeTargets.cmake
    NAMESPACE OrthoTree::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OrthoTree
)

# Configure package config files for find_package()
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/OrthoTreeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/OrthoTreeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/OrthoTreeConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OrthoTree
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OrthoTreeConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OrthoTreeConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OrthoTree
)

# --- Optional: Tests and Benchmarks ---
# This allows building the tests/benchmarks if this is the top-level project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(ORTHOTREE_BUILD_BENCHMARKS "Build benchmarks" OFF)
    option(ORTHOTREE_BUILD_TESTS "Build unit tests" OFF)

    if(ORTHOTREE_BUILD_BENCHMARKS)
        add_subdirectory(benchmarks/automatic)
    endif()

    if(ORTHOTREE_BUILD_TESTS)
        # Note: tests/unittests might need its own CMakeLists.txt update
        # add_subdirectory(tests/unittests)
    endif()
endif()
