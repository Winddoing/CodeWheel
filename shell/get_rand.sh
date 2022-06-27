#!/bin/sh

rand()
{    
	min=$1    
	max=$(($2-$min+1))    
	num=$(cat /dev/urandom | head -n 10 | cksum | awk -F ' ' '{print $1}')    
	echo $(($num % $max + $min))
} 

rnd=$(rand 100 500)
echo $rnd 
min=10
max=50
rnd=$(rand $min $max)
echo $rnd 
