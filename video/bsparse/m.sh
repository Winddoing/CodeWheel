#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2022-11-08 19:56:51
#  Description  :
##########################################################


# sudo apt install libavcodec-dev

set -x

gcc bsparse.c -o bsparse.out `pkg-config --libs --cflags libavcodec libavutil`