#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2025-06-07 14:03:55
#  Description  :
# 	sudo apt install gnuplot
##########################################################


gcc kalman_filter.c

./a.out > tst.csv

gnuplot tst.plot

eog tst.png
