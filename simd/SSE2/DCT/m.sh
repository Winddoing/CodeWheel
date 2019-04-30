#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年04月30日 星期二 14时30分43秒
# Description	:
##########################################################

simd_flag="-mpopcnt -msse -msse2 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx -mavx2 -mbmi"

gcc *.c ${simd_flag} -Wall
