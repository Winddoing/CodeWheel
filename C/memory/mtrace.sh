#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : mtrace.sh
#  Author       : wqshao
#  Created Time : 2024-05-21 17:09:06
#  Description  :
##########################################################

SRC_FILE=t_mtrace.c
EXE_FILE=t_mtrace.out
TRACE_F=t.raw

set -x
gcc $SRC_FILE -o $EXE_FILE

export MALLOC_TRACE=$TRACE_F
./$EXE_FILE

cat $TRACE_F

mtrace ./$EXE_FILE $MALLOC_TRACE
set +x
