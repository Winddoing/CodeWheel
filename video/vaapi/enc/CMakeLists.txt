cmake_minimum_required(VERSION 3.2)
project(enc)

set(enc_sources
	h264encode.c)

add_executable(enc_test ${enc_sources})


target_link_libraries(enc_test ${VA_LIBRARIES} ${DRM_LIBRARIES} va-drm)

target_link_libraries(enc_test common)
target_link_libraries(enc_test pthread m)
