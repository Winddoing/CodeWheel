#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : bash_log.sh
#  Author       : wqshao
#  Created Time : 2023-04-20 14:31:42
#  Description  :
##########################################################

exec 1>mylog 2>&1

set -xv
free
df -h
pwd
