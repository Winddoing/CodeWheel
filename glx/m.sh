#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年12月18日 星期五 17时55分31秒
# Description	:
##########################################################

g++ glx_multi_thread_render.c `pkg-config --libs --cflags x11 glx gl glew`  -lpthread
