#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2022-06-25 18:47:28
#  Description  :
##########################################################

gcc ./mbr_creator.c -o ./mbr_creator

./mbr_creator partitions.tab mbr.bin
