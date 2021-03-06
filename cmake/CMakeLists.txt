cmake_minimum_required(VERSION 3.2)
project(cmake_demo)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake")

set(PACKAGE_VERSION 0.0.1)

# The version number.
set (TEST_VERSION_MAJOR 1)
set (TEST_VERSION_MINOR 0)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else ()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/Config.h.in"
    "${PROJECT_BINARY_DIR}/Config.h"
    )

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${ROOT_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})

link_directories(${ROOT_DIR}/lib)
link_directories(${ROOT_DIR}/externals/lib)

# USE_MYMATH 为编译开关，中间的字符串为描述信息，ON/OFF为默认选项
option (USE_MYMATH
    "Use provided math implementation" OFF)

#find_package(math REQUIRED)

# Test pkg-config
set(CMAKE_CXX_STANDARD 17)
find_package(PkgConfig REQUIRED)
pkg_search_module(GL REQUIRED gl)

message(STATUS "=========pkg-config========")
MESSAGE(STATUS "-L: "${GL_LIBRARIES})
MESSAGE(STATUS "-I: "${GL_INCLUDE_DIRS})
message(STATUS "===========================")


if(USE_MYMATH)
    add_definitions(-DUSE_MYMATH)
    add_subdirectory(${ROOT_DIR}/unit/math)
endif(USE_MYMATH)

add_subdirectory(${ROOT_DIR}/src)

# 打包deb，prefix默认是/usr

install(
    TARGETS test.out
    COMPONENT applications
    DESTINATION "bin"
)

if(USE_MYMATH)
install(
    FILES "unit/math/my_math.h"
    DESTINATION "include"
    COMPONENT headers
)
install(
    TARGETS my_math
    COMPONENT libraries
    DESTINATION "lib"
)
endif(USE_MYMATH)

include(deb)  # build/cmake/deb.cmake
include(rpm)  # build/cmake/rpm.cmake

include(CPack)

