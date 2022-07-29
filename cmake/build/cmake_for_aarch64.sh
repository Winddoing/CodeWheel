##########################################################
# File Name		: cmake.sh
# Author		: winddoing
# Created Time	: 2019年01月14日 星期一 15时56分23秒
# Description	:
##########################################################
#!/bin/bash

cmake -DAARCH64_LINUX=1 -DCMAKE_TOOLCHAIN_FILE=./cmake/cmake_for_aarch64.cmake -G "Unix Makefiles" ..

make
