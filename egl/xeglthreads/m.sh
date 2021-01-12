#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2021年01月12日 星期二 22时35分18秒
# Description	:
##########################################################

set -x

gcc xeglthreads.c -o xeglthreads.out -g `pkg-config --libs --cflags egl gl x11` -lpthread -lm
