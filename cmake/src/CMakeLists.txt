cmake_minimum_required(VERSION 3.2)
project(test)

set(TEST_DIR ${PROJECT_SOURCE_DIR})
message(STATUS "src-dir:${TEST_DIR}")

execute_process(
    COMMAND file main.c
    OUTPUT_FILE ${TEST_DIR}/test.txt)

aux_source_directory(${TEST_DIR} TEST_SRC)

set(EXE_NAME test.out)
add_executable(${EXE_NAME} ${TEST_SRC})

if(USE_MYMATH)
    include_directories(${ROOT_DIR}/unit/math)
    target_link_libraries(${EXE_NAME} my_math m)
else()
    target_link_libraries(${EXE_NAME} m)
endif(USE_MYMATH)

