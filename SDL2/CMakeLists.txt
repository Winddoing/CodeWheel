project (SDL2test)
cmake_minimum_required (VERSION 2.8)

set(SDL2TST_BUILD_DIR ${PROJECT_SOURCE_DIR}/build)                      
set(CMAKE_BINARY_DIR ${SDL2TST_BUILD_DIR})                              
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SDL2TST_BUILD_DIR}/lib)            
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SDL2TST_BUILD_DIR}/lib)            
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SDL2TST_BUILD_DIR}/bin)            

message(STATUS "xxxxxxxxxxxxxxxxxxxxxxxxx")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "SDL2TST_BUILD_DIR: ${SDL2TST_BUILD_DIR}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "xxxxxxxxxxxxxxxxxxxxxxxxx")


link_libraries(SDL2 png)
add_executable(png2yuv png2yuv.c)
file(COPY test.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY test.bmp DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(sdl2-test sdl2-test.c)

add_executable(play_bgra play_bgra.c)
file(COPY test_bgra_320x180.rgb DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
