# vim: syntax=cmake
SET(CMAKE_SYSTEM_NAME Linux)
# specify the cross compiler
#SET(TOOLCHAIN_PATH "${PROJECT_SOURCE_DIR}/../../prebuilts/gcc/aarch64-none-linux-gnu/bin")
#SET(CMAKE_C_COMPILER   "${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-gcc")
#SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-g++")
SET(CMAKE_C_COMPILER   "aarch64-none-linux-gnu-gcc")
SET(CMAKE_CXX_COMPILER "aarch64-none-linux-gnu-g++")

# SET(CMAKE_FIND_ROOT_PATH /opt/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

