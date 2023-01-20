#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2023-01-19 16:38:54
#  Description  :
##########################################################


set -x

gcc colorspace.c -o colorspace.out
./colorspace.out

gcc bgra_convert.c -o bgra_convert.out
./bgra_convert.out 320 240 font_320x240.bgr0

set +x
