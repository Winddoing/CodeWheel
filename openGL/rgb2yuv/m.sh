#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2023-01-17 09:58:00
#  Description  :
##########################################################


set -x

gcc main.cpp -o rgb2nv12.out `pkg-config --libs --cflags libdrm epoxy gbm`

set +x
