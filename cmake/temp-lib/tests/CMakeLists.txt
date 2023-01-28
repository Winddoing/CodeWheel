cmake_minimum_required(VERSION 3.2)
project(TEMP_test)

set(TST_BIN TEMP_test)
add_executable(${TST_BIN} TEMP_test.c)
#target_link_libraries(${TST_BIN} TEMP-shared)
target_link_libraries(${TST_BIN} TEMP-static)
