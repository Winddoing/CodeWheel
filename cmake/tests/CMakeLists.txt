cmake_minimum_required(VERSION 3.2)
project(test_demo)

set(TEST_DIR ${PROJECT_SOURCE_DIR})
message(STATUS "src-dir:${TEST_DIR}")

set(EXE_NAME tst_demo)

aux_source_directory(${TEST_DIR} TEST_SRC)
add_executable(${EXE_NAME} ${TEST_SRC})

