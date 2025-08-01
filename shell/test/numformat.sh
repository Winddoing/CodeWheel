#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : numformat.sh
#  Author       : wqshao
#  Created Time : 2024-08-29 09:44:27
#  Description  :
##########################################################


num=$1  #16进制

hex=$(printf "%s" $num)
dec=$(printf "%d" 0x$num)
oct=$(printf "%o" 0x$num)
bin=$(printf "%b" $num)
asc=$(echo $num | xxd -r -p)

echo -e "\tHex     : $hex"
echo -e "\tDecimal : $dec"
echo -e "\tOctal   : $oct"
#echo -e "\tBinary  : $bin"
echo -e "\tASCii   : $asc"
