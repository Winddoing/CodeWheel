#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2022-12-13 16:24:43
#  Description  :
##########################################################

top=$(pwd)
ARCH=$(uname -m)
if [ ! -d "build" ]; then
	mkdir build
fi

if [ $# -eq 0 ]; then
	args_list="-clbip"
else
	args_list=$@
fi

echo "Args list: $args_list"

cd build
set -- $(getopt -q dlbcirp "$args_list")
while [ -n "$1" ]
do
	case "$1" in
		-d) echo "Reload the CMakeList files for Debug"
			cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..
			shift ;;
		-l) echo "Reload the CMakeList files for Release"
			#cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
			cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -DUSE_MYMATH=1 ..
			shift ;;
		-b) echo "Make Project"
			cmake --build . -- -j 8
			# cmake --build . --target test -- -j 8
			shift ;;
		-c) echo "clean project"
			cmake --build . --target clean --
			if [ ${PWD##*/} = "build" ]; then
				rm -rf *
			fi
			shift ;;
		-i) echo "Install lib"
			rm $top/packages/$ARCH/* -rf
			cmake --install .
			#make install
			shift ;;
		-r) echo "Run test, $top"
			ulimit -c unlimited
			./bin/test.out 12
			./bin/tst_demo
			shift ;;
		-p) echo "Pack "
			if (( $(cat /etc/*-release | grep -wiE 'CentOS' | wc -l) > 0 )); then
				echo "for CentOS"
				cpack -G RPM
			else
				echo "for Ubuntu"
				cpack -G DEB
			fi
			shift ;;
		--) shift
			break ;;
		-*) echo "Nothing to do";;
	esac
done
cd $top
