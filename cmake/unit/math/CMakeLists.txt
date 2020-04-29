cmake_minimum_required(VERSION 3.2)
project(my_math)

set(MATH_DIR ${PROJECT_SOURCE_DIR})

aux_source_directory(${MATH_DIR} DIR_SRCS)

set(my_math ${LIB_NAME})

add_library(my_math STATIC ${DIR_SRCS})
#add_library(my_math SHARED ${DIR_SRCS})

