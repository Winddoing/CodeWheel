#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : devmem.sh
#  Author       : wqshao
#  Created Time : 2025-04-22 11:25:01
#  Description  :
##########################################################

BASE=0xF1404000

for addr in `seq 0 1024`
do
	[ $((addr % 4)) -eq 0 ] || continue;

	addr=$(printf "0x%x" $addr)
	#echo "---addr=$addr"
	#if [ "$addr" = "0x38" ]; then
	#	continue
	#fi
	#for ch in `seq 1 10`
	#do
	#	w1addr=`printf "0x%x" $((0x148 + ($ch-1) * 0x100))`
	#	w2addr=`printf "0x%x" $((0x198 + ($ch-1) * 0x100))`
	#	if [ "$addr" = "$w1addr" ] || [ "$addr" = "$w2addr" ]; then
	#		continue
	#	fi
	#done

	addr=`printf "0x%x\n" $(($BASE + $addr))`
	echo -n "addr=$addr : "

	devmem $addr
done
