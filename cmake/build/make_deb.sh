#!/bin/bash
##########################################################
# File Name		: make_deb.sh
# Author		: winddoing
# Created Time	: 2020年11月18日 星期三 11时30分29秒
# Description	:
##########################################################

set -x

./cmake_mymath.sh

cpack


dpkg --info ./cmake_demo_0.0.1_amd64.deb

dpkg -c ./cmake_demo_0.0.1_amd64.deb
