#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年04月30日 星期二 15时04分24秒
# Description	:
##########################################################

#gcc -Wall  -mmmx -o ins.out  mmx_ins.c
gcc -Wall mmx_ins.c -o ins.out
gcc -Wall mmx_add.c -o add.out
