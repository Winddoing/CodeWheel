cmake_minimum_required(VERSION 3.2)
project(TEMP)

set(CMAKE_BUILD_TYPE "Debug")

# 编译时参数
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Wextra")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-discarded-qualifiers")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections") #标记暂时不用的功能函数,与-Wl,--gc-sections配合使用

# 可执行文件链接时参数
# -Wl,--gc-sections： 不链接未用函数，减小可执行文件大小
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_definitions( -DCOMMIT_HASH=\"${COMMIT_HASH}\")

execute_process(
    COMMAND git symbolic-ref --short -q HEAD
    OUTPUT_VARIABLE BRANCH_NAME
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_definitions( -DBRANCH_NAME=\"${BRANCH_NAME}\")

string(TIMESTAMP COMPILE_TIME %Y%m%d%H)
add_definitions(-DCOMPILE_TIME=\"${COMPILE_TIME}\")

execute_process(
	COMMAND uname -m COMMAND xargs echo -n
	OUTPUT_VARIABLE ARCHITECTURE
)

message(STATUS " COMMIT_HASH : ${COMMIT_HASH}")
message(STATUS " BRANCH_NAME : ${BRANCH_NAME}")
message(STATUS "COMPILE_TIME : ${COMPILE_TIME}")
message(STATUS "     VERSION : ${LIB_VERSION}")
message(STATUS "Architecture : ${ARCHITECTURE}")

file(WRITE version.info "${COMPILE_TIME}_${ARCHITECTURE}_${LIB_VERSION}|${BRANCH_NAME}-${COMMIT_HASH}\n")

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${ROOT_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})

if (${X86_LINUX})
	message(STATUS "Build X86")
	set(CMAKE_CXX_STANDARD 17)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(ALSA REQUIRED alsa)
	message(STATUS "=========pkg-config========")
	message(STATUS "-L: ${ALSA_LIBRARIES}")
	message(STATUS "-I: ${ALSA_INCLUDE_DIRS}")
	message(STATUS "===========================")
elseif (${AARCH64_LINUX})
	message(STATUS "Build aarch64")
	include_directories(${ROOT_DIR}/externals/include)
	link_directories(${ROOT_DIR}/externals/lib/aarch64-linux)
endif()

add_subdirectory(${ROOT_DIR}/src)
add_subdirectory(${ROOT_DIR}/tests)

# 指定安装目录，默认路径：/usr/local/lib/
set(CMAKE_INSTALL_PREFIX ${ROOT_DIR}/packages/${ARCHITECTURE})

set(ALL_TARGETS TEMP-shared TEMP_test)
set_target_properties(TEMP-shared PROPERTIES PUBLIC_HEADER ${ROOT_DIR}/include/TEMP.h)
# CMAKE_BUILD_TYPE=Debug时导出
install(TARGETS ${ALL_TARGETS}
	CONFIGURATIONS Debug
	LIBRARY DESTINATION Debug/lib
	ARCHIVE DESTINATION Debug/lib
	RUNTIME DESTINATION Debug/bin
	PUBLIC_HEADER DESTINATION Debug/include
	)
install(FILES version.info
	CONFIGURATIONS Debug
	SYSCONF DESTINATION Debug
	)

# CMAKE_BUILD_TYPE=Release时导出
install(TARGETS ${ALL_TARGETS}
	CONFIGURATIONS Release
	LIBRARY DESTINATION Release/lib
	ARCHIVE DESTINATION Release/lib
	RUNTIME DESTINATION Release/bin
	PUBLIC_HEADER DESTINATION Release/include
	)
install(FILES version.info
	CONFIGURATIONS Release
	SYSCONF DESTINATION Release
	)
