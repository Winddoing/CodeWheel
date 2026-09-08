#!/bin/bash
##########################################################
# Copyright (C) 2026 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2026-03-30 14:52:14
#  Description  :
##########################################################

set -x

gcc tft_printf.c -o tft_printf.out
nm --print-size tft_printf.out  | grep -E 'printf|doprntx'
#0000000000001b74 00000000000000eb t my_printf
#0000000000001303 000000000000084e T __tfp_printf
# 大小：0xeb + 0x84e = 0x939 = 2.3KB

gcc doprntx.c -o doprntx.out
nm --print-size doprntx.out | grep -E 'printf|doprntx'
#0000000000001210 0000000000000f02 t _doprntx
#0000000000002135 00000000000000e3 t my_printf
# 大小：0xf02 + 0xe3 = 0xFE5 = 3.9KB

