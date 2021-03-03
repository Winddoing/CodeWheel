#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年10月23日 星期五 17时09分37秒
# Description	:
##########################################################

set -x

# sudo apt install libgbm-dev

gcc egl_off-scree_rendering.c -o egl_off-scree_rendering.out `pkg-config --libs --cflags egl gbm gl`


# sudo apt install libdrm-dev
gcc demo.c -o demo.out -g `pkg-config --libs --cflags egl gbm gl libdrm`



gcc ./xeglthreads.c -o ./xeglthreads.out -g `pkg-config --libs --cflags egl gbm gl libdrm x11` -lpthread -lm


gcc egl_gbm_render.c -o egl_gbm_render.out `pkg-config --libs --cflags egl gl gbm`
