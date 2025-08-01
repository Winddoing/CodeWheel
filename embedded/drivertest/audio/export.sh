#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : export.sh
#  Author       : wqshao
#  Created Time : 2025-06-10 15:10:23
#  Description  :
##########################################################

set -x
gcc audio_convert.c -o audio_convert
strip audio_convert
mv audio_convert ~/.work_env/tools/x86/
set +x
