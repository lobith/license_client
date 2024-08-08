cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

# ================================================================================
# foleys_license_client
# ================================================================================


set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(foleys_licensing_client
        VERSION 0.0.1)

# src contains the actual library
add_subdirectory(src)

# various examples
#add_subdirectory(examples/juce_plugin)
add_subdirectory(examples/minimal)
