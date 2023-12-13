#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2023-12-06 10:59:53
#  Description  :
##########################################################


[ -d build ] || mkdir build

cd build
if [ -z ${ANDROID_NDK_ROOT} ]; then                                 
	echo  "You must set ANDROID_NDK_ROOT environment variable"      
	echo " Please download NDK r21d from https://dl.google.com/android/repository/android-ndk-r21d-linux-x86_64.zip"
	exit                                                            
fi                                                                  

cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_NDK=${ANDROID_NDK_ROOT} \
    -DANDROID_NATIVE_API_LEVEL=20 \
	..

make VERBOSE=on

cd -

