#!/bin/bash
##########################################################
# File Name		: make_deb.sh
# Author		: winddoing
# Created Time	: 2020年11月18日 星期三 11时30分29秒
# Description	:
##########################################################

set -x

./clean.sh

./cmake_mymath.sh

#cpack
cpack -G DEB


dpkg --info ./test_demo_0.0.1._amd64.deb

dpkg -c ./test_demo_0.0.1._amd64.deb
