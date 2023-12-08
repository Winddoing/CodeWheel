cmake_minimum_required(VERSION 3.2)
project(common)

add_definitions( -DHAVE_VA_DRM)

set(common_sources
	va_display.c
	va_display_drm.c)

add_library(common STATIC ${common_sources})

target_link_libraries(common ${DRM_LIBRARIES} ${VA-DRM_LIBRARIES})
