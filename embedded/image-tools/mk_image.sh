#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : mk_image.sh
#  Author       : wqshao
#  Created Time : 2022-06-23 16:42:28
#  Description  : 将uboot，kernel制作成一个整体，方便使用
#   实际使用： dd if=mmc_image.bin of=/dev/sdb
##########################################################

mk_image()
{
	local img_path=$1
	local allimg=${img_path}/$2

	echo "Images path: $img_path"
	echo "allimg: $allimg"

	local image_table=(
		# name   file            offset
		"gpt	 mbr-gpt.bin     0"
		"spl     spl.img         64"
		"uboot   u-boot.itb      1088"
		"kernel  boot.img        4096"
		"rootfs  rootfs.squashfs 20480"
	)
	# offset unit is block, 512Byte

	echo "Generate the overall image."
	dd if=/dev/zero of=$allimg bs=1M count=1 2> /dev/null

	echo "Init the overall image."
	for image in "${image_table[@]}"
	do
		img_line=($image)
		img_name=${img_line[0]}
		img_file=${img_line[1]}
		img_ofst=${img_line[2]}

		img_file=${img_path}/${img_file}


		if [ ! -f $allimg ]; then
			echo "$allimg not exist!"
			exit
		fi

		if [ ! -f $img_file ];then 
			echo "$img_file not exist!"
			break
		fi

		echo " Write $img_name ..."
		dd if=$img_file of=$allimg seek=$img_ofst 2> /dev/null
		if [ $? -ne 0 ]; then
			echo "[xxx] write $img_name error, img:$img_file"
			exit
		fi
	done
	sync
	echo "Done"
}


mk_image $PWD mmc_image.bin


