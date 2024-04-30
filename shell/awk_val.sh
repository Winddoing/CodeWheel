#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : awk_val.sh
#  Author       : wqshao
#  Created Time : 2024-04-29 16:21:45
#  Description  :
##########################################################

echo "apple banana orange" | awk '{print $1,$2,$3}' | while read a b c
do
   echo  a=$a b=$b c=$c
done

TEST01(){
    count=0
    while [[ $count < 100 ]];do
        memory=`free -m | grep Mem`
        read total used buffers cached <<< `echo "$memory" |awk '{print $2,$3,$6,$7}'`
        count=`echo "$count + 1" | bc`
    done
}

TEST02(){
    count=0
    while [[ $count < 100 ]];do
        memory=`free -m | grep Mem`
        total=`echo "$memory" |awk '{print $2}'`
        used=`echo "$memory" |awk '{print $3}'`
        buffers=`echo "$memory" |awk '{print $6}'`
        cached=`echo "$memory" |awk '{print $7}'`
        count=`echo "$count + 1" | bc`
    done

}

echo -n "全部一次性:"
time TEST01


echo ""
echo -n "不是一次性:"
time TEST02
# echo "total:$total used:$used buffers:$buffers cached:$cached"
