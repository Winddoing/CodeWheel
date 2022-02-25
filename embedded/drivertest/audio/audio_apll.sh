#!/bin/bash
##########################################################
# File Name		: audio_apll.sh
# Author		: winddoing
# Created Time	: 2022年02月24日 星期四 15时09分06秒
# Description	:
##########################################################

gcd=
lcm=

# 最大公约数
GCD()
{
    a=$1
    b=$2
    if [ $a -lt $b ];then
        max=$b
        min=$a
    else
        max=$a
        min=$b
	fi
    if [ $(($max%$min)) -ne 0 ];then
        GCD $min $(($max%$min))
    fi
    gcd=$min
}

# 最小公倍数
LCM()
{
    GCD $1 $2
    b=$(($1*$2))
    lcm=$(($b/$gcd))
}

mclk_array=()
mclk_fs=256
i=0
#for rate in 8000 11025 16000 22050 32000 44100 48000 64000 88200 96000 176400 192000
for rate in 8000 11025 16000 22050 32000 44100 48000 64000 88200 96000 192000
do
	#rate=$((rate / 1000))
	mclk=$((rate * mclk_fs))
	echo "---i=$i, rate=$rate, aaa=$mclk"
	mclk_array[$i]=$mclk
	i=$((i+1))
done

aalcm=${mclk_array[0]}
for((i=0;i<${#mclk_array[@]};i++))
do
	echo "----m1:${mclk_array[i]}, ${mclk_array[i+1]}"
	if [ -z ${mclk_array[i+1]} ]; then
		break
	fi
	echo "---aaaaaaaaaa-aaLcm=$aalcm"
	LCM $aalcm ${mclk_array[i+1]}
	aalcm=$lcm
	echo "----aaLcm=$aalcm"
done

echo "apll=$aalcm HZ [$((aalcm / 1000 / 1000))]"

