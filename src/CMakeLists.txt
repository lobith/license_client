cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

# ================================================================================
# foleys_license_client implementation
# ================================================================================

include(FetchContent)

# cpr is a wrapper around libcurl
option(CURL_ZLIB OFF)
FetchContent_Declare(cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_SHALLOW ON)
FetchContent_MakeAvailable(cpr)

include(Sodium)

add_library(licensing
        INTERFACE
        foleys_License.cpp
        foleys_License.h
        foleys_SharedObject.h)


target_link_libraries(licensing
        INTERFACE
        cpr::cpr
        sodium)
