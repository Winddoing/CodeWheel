#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : os_info.sh
#  Author       : wqshao
#  Created Time : 2022-09-20 14:01:06
#  Description  :
##########################################################

BUILD_LOG="os_info.log"

echo "OS info:" > $BUILD_LOG

echo "" >> $BUILD_LOG
echo "===> kernel info: " >> $BUILD_LOG
uname -a >> $BUILD_LOG

echo "" >> $BUILD_LOG
echo "===> gcc version:" >> $BUILD_LOG
gcc -v &>> $BUILD_LOG

echo "" >> $BUILD_LOG
echo "===> system info:" >> $BUILD_LOG
cat /proc/version >> $BUILD_LOG

echo "" >> $BUILD_LOG
echo "===> CPU info:" >> $BUILD_LOG
cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c >> $BUILD_LOG

echo "" >> $BUILD_LOG
echo "===> code info: " >> $BUILD_LOG
git log -n 1 --branches --format=oneline >> $BUILD_LOG

echo "" >> $BUILD_LOG
echo "===> os release:" >> $BUILD_LOG
cat /etc/*-release >> $BUILD_LOG
