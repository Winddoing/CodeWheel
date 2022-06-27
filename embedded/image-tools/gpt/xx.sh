#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : xx.sh
#  Author       : wqshao
#  Created Time : 2022-06-23 11:00:59
#  Description  :
##########################################################

gcc ./gpt_creator.c -o ./gpt_creator -lz

./gpt_creator partitions.tab mbr-of-gpt.bin gpt.bin

cat mbr-of-gpt.bin gpt.bin > mbr-gpt.bin

dd if=/dev/zero of=file.bin bs=512 count=32   #gpt分区表，最大支持128个分区

cat mbr-gpt.bin file.bin > file2.bin

dd if=file2.bin of=mbr-gpt.bin bs=512 count=33 #1个mbr头+32个gpt分区

rm file* -rf
rm mbr-of-gpt.bin gpt.bin -rf
