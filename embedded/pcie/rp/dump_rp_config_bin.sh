#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : dump_rp_config_bin.sh
#  Author       : wqshao
#  Created Time : 2025-09-23 11:00:03
#  Description  :
##########################################################

set -x

#hexdump -v -e '"%.3_ax:" 1/4 "%08x ""\n"' ./rp_config.bin
hexdump -e '"%.3_ax:" 1/4 "%08x ""\n"' ./rp_config.bin
