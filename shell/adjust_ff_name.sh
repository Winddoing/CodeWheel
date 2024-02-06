#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : adjust_ff_name.sh
#  Author       : wqshao
#  Created Time : 2024-01-20 14:26:34
#  Description  :
##########################################################
TOP=`pwd`

OLD_STR="Exynos"
NEW_STR="Mmedia"

OLD_STR1="EXYNOS"
NEW_STR1="MMEDIA"

OLD_STR2="exynos"
NEW_STR2="mmedia"

rename_file_name()
{
	find . -path ./.git -prune -o -name '*' -print |
		while read file_name; do
			if [[ $file_name =~ $OLD_STR ]]; then
				new_file_name=`echo $file_name | sed "s/$OLD_STR/$NEW_STR/g"`
				#echo "file name=$file_name, new name: $new_file_name"
				set -x
				mv $file_name $new_file_name
				set +x
			fi
		done
}

rename_func_name()
{
	echo "rename func name."
	find . -path ./.git -prune -o -name '*' -print |
		while read file_name; do
			if [ $file_name = $0 ]; then
				echo "no process"
				continue
			fi
			if [ -f $file_name ]; then
				echo "file: $file_name"
				sed -i "s/$OLD_STR/$NEW_STR/g" $file_name
				sed -i "s/$OLD_STR1/$NEW_STR1/g" $file_name
				sed -i "s/$OLD_STR2/$NEW_STR2/g" $file_name
				sed -i "s/2012 Samsung/2024 xxx/g" $file_name
				sed -i "s/2012.02.20/2024.01.20/g" $file_name
			fi
		done
}

rename_func_name
rename_file_name
