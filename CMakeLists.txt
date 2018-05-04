cmake_minimum_required(VERSION 3.1)
project("RetroSpyServer")

if (WIN32)
add_definitions("-DWIN32 -D__WIN32__")
endif()

find_package(Boost REQUIRED)

set(WITH_JDBC ON)

# LibUV
add_subdirectory(libuv-cmake)
include_directories("libuv-cmake/libuv/include")

# MySQL
add_subdirectory(mysql-connector-cpp)
include_directories("mysql-connector-cpp/jdbc")
include_directories("${CMAKE_BINARY_DIR}/mysql-connector-cpp/jdbc/cppconn")

# Boost
include_directories(${Boost_INCLUDE_DIR})

# Set Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add modules
add_subdirectory("common")
add_subdirectory("playersearch")
add_subdirectory("playerspy")
add_subdirectory("RetroSpy")
