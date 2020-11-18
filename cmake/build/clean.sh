##########################################################
# File Name		: clean.sh
# Author		: winddoing
# Created Time	: 2019年01月14日 星期一 15时55分59秒
# Description	:
##########################################################
#!/bin/bash

rm -rf CMakeCache.txt  CMakeFiles  cmake_install.cmake Makefile lib  bin Config.h  src  unit *.deb

rm -rf _CPack_Packages CPackConfig.cmake  CPackSourceConfig.cmake  install_manifest.txt
