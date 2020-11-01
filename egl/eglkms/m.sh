#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年11月01日 星期日 21时22分12秒
# Description	:
##########################################################

gcc eglkms.c -o eglkms.out -g `pkg-config --libs --cflags libdrm egl gl gbm`
