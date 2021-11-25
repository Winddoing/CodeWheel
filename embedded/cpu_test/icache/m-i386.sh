#!/bin/bash
##########################################################
# File Name		: m-i386.sh
# Author		: winddoing
# Created Time	: 2021年11月25日 星期四 16时57分32秒
# Description	:
##########################################################


# sudo apt install gcc-multilib


gcc -m32 flushtest.c -o icache -lpthread -static


