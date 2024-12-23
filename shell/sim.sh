#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : sim.sh
#  Author       : wqshao
#  Created Time : 2024-12-23 09:18:49
#  Description  :
##########################################################

test1="12345"
test2="12344"

test1=AQAAO0mUaEkSawIAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
test2="AQAAO0mUJEuSWBMOAAAAAMABHAAAAAAAAAAAAAcOAAAAAAAhhBAA"


if [ -L /sbin/fdisk ]; then
	d=$(diff <(echo "$test1" |sed 's/./&\n/g') <(echo "$test2" |sed 's/./&\n/g') | grep -c '^[<>]')
else
	d=$(diff <(echo "$test1" |sed 's/./&\n/g') <(echo "$test2" |sed 's/./&\n/g') | grep -c '^[-+]')
fi

echo "--d=$d"

echo $((100-100*d/(${#test1}+${#test2})))%
