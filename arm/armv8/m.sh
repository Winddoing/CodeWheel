#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2024-08-26 11:09:23
#  Description  :
##########################################################

set -x

aarch64-linux-gnu-gcc -static armv8_timer_cntvct.c -o armv8_timer_cntvct.out

set +x
