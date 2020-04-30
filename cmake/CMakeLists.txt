cmake_minimum_required(VERSION 3.2)
project(cmake_demo)

# The version number.
set (TEST_VERSION_MAJOR 1)
set (TEST_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/Config.h.in"
    "${PROJECT_BINARY_DIR}/Config.h"
    )

# should we use our own math functions
#option(USE_MYMATH "Use tutorial provided math implementation" ON)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${ROOT_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})

link_directories(${ROOT_DIR}/lib)
link_directories(${ROOT_DIR}/externals/lib)

option (USE_MYMATH "Use provided math implementation" OFF)

if(USE_MYMATH)
    add_definitions(-DUSE_MYMATH)
    add_subdirectory(${ROOT_DIR}/unit/math)
endif(USE_MYMATH)

add_subdirectory(${ROOT_DIR}/src)
