#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : pack_data.sh
#  Author       : wqshao
#  Created Time : 2023-04-30 14:35:46
#  Description  :
##########################################################


OUTPUT_DIR="data_package"

type 7z-comp > /dev/null
if [ $? -ne 0 ]; then
	echo "There is no 7z-comp command."
	exit
fi

type ch2py > /dev/null
if [ $? -ne 0 ]; then
	echo "There is no ch2py command."
	echo "	sudo apt install ruby"
	echo "	sudo gem install chinese_pinyin"
	exit
fi

[ -d $OUTPUT_DIR ] || mkdir $OUTPUT_DIR

for dir in `ls ./`
do
	if [ "$dir" = "$OUTPUT_DIR" ]; then
		echo "ignore dir=$dir"
		continue
	fi
	if [ ! -d $dir ]; then
		echo "not dir. dir=$dir"
		continue
	fi

	dir_pinyin=$(ch2py $dir -c -s _)
	dir_pack_name=$(date +%Y%m%d)_${dir_pinyin}.bin

	echo "=====> dir=$dir, dir_pinyin=$dir_pinyin, dir_pack_name=$dir_pack_name"

	7z-comp $OUTPUT_DIR/$dir_pack_name $dir
	if [ $? -ne 0 ]; then
		echo "$dir compression error."
		break;
	fi
done
