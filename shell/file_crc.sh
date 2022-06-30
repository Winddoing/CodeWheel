#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : file_crc.sh
#  Author       : wqshao
#  Created Time : 2022-06-28 15:19:07
#  Description  :
##########################################################
set -x

cksum tst.yaml | xxd -r -ps > file0.bin
dd if=/dev/zero of=file1.bin bs=100 count=1
cat file0.bin file1.bin > file2.bin
dd if=file2.bin of=file1.bin bs=100 count=1
