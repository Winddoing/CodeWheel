cmake_minimum_required(VERSION 3.2)
project(test)

set(TEST_DIR ${PROJECT_SOURCE_DIR})
message("src-dir:${TEST_DIR}")

execute_process(                                                            
    COMMAND file main.c   
    OUTPUT_FILE ${TEST_DIR}/test.txt)                               

aux_source_directory(${TEST_DIR} TEST_SRC)

set(EXE_NAME test.out)
add_executable(${EXE_NAME} ${TEST_SRC})

target_link_libraries(${EXE_NAME} m)
