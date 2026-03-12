# CPM.cmake - CMake Package Manager
# Copyright (c) 2019-2023 Lars Melchior and contributors
# Licensed under the MIT License
# https://github.com/cpm-cmake/CPM.cmake

set(CPM_DOWNLOAD_VERSION 0.40.0)
set(CPM_DOWNLOAD_LOCATION
    "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake"
    CACHE INTERNAL ""
)

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
       ${CPM_DOWNLOAD_LOCATION}
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})
