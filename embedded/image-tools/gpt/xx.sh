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

dd if=/dev/zero of=file.bin bs=512 count=63

cat mbr-gpt.bin file.bin > file2.bin

dd if=file2.bin of=mbr-gpt.bin bs=512 count=64

rm file* -rf
rm mbr-of-gpt.bin gpt.bin -rf
