#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : data_unit.sh
#  Author       : wqshao
#  Created Time : 2024-04-30 15:11:10
#  Description  :
##########################################################

input=$1

data2byte()
{
	local data=$1

	#判断输入是否为数字,如为数字直接显示
	if [ -z "${data//[0-9]/}" ]; then
		value=${data}
	else
		#否则，取输入参数的最后一位为单位
		lastchar=${data#${data%?}}
		#并获取除去单位的数字,即去掉最后一位
		num=${data%${lastchar}}
		#如果最后一位为K,M,G
		case $lastchar in
			K|k)
				value=$(($num * 1024))
				;;
			m|M)
				value=$(($num * 1024 * 1024))
				;;
			g|G)
				value=$(($num * 1024 * 1024 * 1024))
				;;
			*)
				#其它单位则报错
				echo "Wrong unit"
				exit 1
				;;
		esac
	fi

	echo "$value"
}

bb=`data2byte $input`

echo "$input -> $bb Byte"

