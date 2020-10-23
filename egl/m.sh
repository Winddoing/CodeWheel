#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年10月23日 星期五 17时09分37秒
# Description	:
##########################################################

gcc egl_off-scree_rendering.c `pkg-config --libs --cflags egl gbm gl`
