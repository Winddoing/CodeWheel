#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年10月23日 星期五 17时09分37秒
# Description	:
##########################################################

# sudo apt install libgbm-dev

gcc egl_off-scree_rendering.c -o egl_off-scree_rendering.out `pkg-config --libs --cflags egl gbm gl`


# sudo apt install libdrm-dev
gcc demo.c -o demo.out -g `pkg-config --libs --cflags egl gbm gl libdrm`
