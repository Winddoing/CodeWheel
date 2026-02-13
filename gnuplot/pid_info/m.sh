#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2025-12-12 09:13:54
#  Description  :
##########################################################

set -x

# while [ 1 ];do cat /proc/499/statm | tee -a statm.txt; sleep 60; done

gnuplot statm.plot
gnuplot statm_mult.plot
