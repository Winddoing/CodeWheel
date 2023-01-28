cmake_minimum_required(VERSION 3.2)
project(TEMP-lib)

set(LIB_NAME "TEMP")
set(LIB_VERSION 0.0.1)
set(DEPEND_LIB "")

set(CUR_DIR ${PROJECT_SOURCE_DIR})

aux_source_directory(${CUR_DIR} CUR_DIR_SRCS)

# 生成动态/静态库
add_library(${LIB_NAME}-shared SHARED ${CUR_DIR_SRCS})
add_library(${LIB_NAME}-static STATIC ${CUR_DIR_SRCS})

# 指定静态库的输出名称
set_target_properties(${LIB_NAME}-shared PROPERTIES OUTPUT_NAME ${LIB_NAME})
set_target_properties(${LIB_NAME}-static PROPERTIES OUTPUT_NAME ${LIB_NAME})

# 指定库版本
# VERSION指代动态库版本，SOVERSION指代API版本
set_target_properties(${LIB_NAME}-shared PROPERTIES VERSION ${LIB_VERSION} SOVERSION 0)
set_target_properties(${LIB_NAME}-static PROPERTIES VERSION ${LIB_VERSION} SOVERSION 0)

target_link_libraries(${LIB_NAME}-shared ${DEPEND_LIB})
target_link_libraries(${LIB_NAME}-static ${DEPEND_LIB})

