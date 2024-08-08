cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

# ================================================================================
# libsodium and CMake build
# ================================================================================


# decryption
include(ExternalProject)

set(SODIUM_PATH ${CMAKE_BINARY_DIR}/_deps/libsodium)

ExternalProject_Add(libsodium
        PREFIX ${SODIUM_PATH}
        URL https://download.libsodium.org/libsodium/releases/LATEST.tar.gz
        CONFIGURE_COMMAND ${SODIUM_PATH}/src/libsodium/configure --prefix=${CMAKE_BINARY_DIR}/libsodium --disable-shared
        BUILD_COMMAND make)


add_library(sodium STATIC IMPORTED)
add_dependencies(sodium libsodium)

set_property(TARGET sodium PROPERTY
        IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/libsodium/lib/libsodium.a")

target_include_directories(sodium INTERFACE "${CMAKE_BINARY_DIR}/libsodium/include")
