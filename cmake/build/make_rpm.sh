#!/bin/bash
##########################################################
# File Name		: make_deb.sh
# Author		: winddoing
# Created Time	: 2020年11月18日 星期三 11时30分29秒
# Description	:
##########################################################

set -x

#./cmake_mymath.sh
cmake3 .. -DUSE_MYMATH=1

#cpack3
cpack3 -G RPM


rpm -qip cmake_demo-0.0.1-1.x86_64.rpm

rpm -qlp cmake_demo-0.0.1-1.x86_64.rpm
