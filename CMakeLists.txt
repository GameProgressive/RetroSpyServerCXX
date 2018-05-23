cmake_minimum_required(VERSION 3.1)
project("RetroSpyServer")

# Add definitions for w32 build that dosen't support _WIN32 macro
if (WIN32)
	add_definitions("-DWIN32 -D__WIN32__")
endif()

# Set -fPIC to allow compiling retrospy shared library with libuv
if (NOT WIN32)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Boost inclusions
#find_package(Boost REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})

# LibUV inclusions
add_subdirectory(libuv-cmake)
include_directories("libuv-cmake/libuv/include")

# Mariadb inclusion
set(WITH_UNIT_TESTS OFF) # Remove useless tests from mariadb build
add_subdirectory(mariadb-connector-c)
include_directories("mariadb-connector-c/include")
include_directories("${CMAKE_BINARY_DIR}/mariadb-connector-c/include") # mariadb config

# Set CMake output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# RetroSpy projects
add_subdirectory("common")
add_subdirectory("playersearch")
add_subdirectory("playerspy")
add_subdirectory("RetroSpy")
