#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2022-12-13 16:24:43
#  Description  :
##########################################################

pin=$(pwd)
if [ ! -d "build" ]; then
  mkdir build
fi
cd build
set -- $(getopt -q dlbcih "$@")
while [ -n "$1" ]
do
        case "$1" in
            -d) echo "Reload the CMakeList files for X86"
                cmake -DX86_LINUX=1 -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
                shift ;;
            -l) echo "Reload the CMakeList files for aarch64"
                cmake -DAARCH64_LINUX=1 -DCMAKE_TOOLCHAIN_FILE=./scripts/cmake_for_aarch64.cmake -G "Unix Makefiles" ..
                shift ;;
            -b) echo "Make Project"
                cmake --build . -- -j 8
                # cmake --build . --target test_decode -- -j 8
                shift ;;
            -c) echo "clean project"
                cmake --build . --target clean --
                if [ ${PWD##*/} = "build" ]; then
                        rm -rf *
                fi
                shift ;;
			-i) echo "Install lib"
				cmake --install .
				#make install
                shift ;;
			-h) echo "Help build cmd:"
				echo "   x86: $0 -cdb"
				echo " arm64: $0 -cld"
                shift ;;
            --) shift
                break ;;
            -*) echo "Nothing to do";;
        esac
done
cd $pin
