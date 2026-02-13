#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : sd_burn_image.sh
#  Author       : wqshao
#  Created Time : 2022-06-23 16:42:28
#  Description  :
##########################################################

PWD=`pwd`
IMG_DEV_NAME=$1
IMG_CELL=$2

[ ${UID} -ne 0 ] && echo "Please run with sudo" && exit -1

if [ $# -lt 1 ]; then
    echo "Usage: $0 <device|img_name>"
    echo "example: "
	echo "	$0 /dev/sdb					#burn all images"
	echo "	$0 /dev/sdb [gpt|spl|uboot|kernel|rootfs]	#Burn a separate image"
    exit
fi

echo $IMG_DEV_NAME | grep "dev"
if [ $? -eq 0 ]; then
	echo "SD card burn..."

	if [ ! -e $IMG_DEV_NAME ];then
		echo "The USB card reader $IMG_DEV_NAME is not present!"
		exit
	fi

	sudo fdisk -l $IMG_DEV_NAME &> /dev/null
	if [ $? -ne 0 ]; then
		echo "The sd card $IMG_DEV_NAME is not present!"
		exit
	fi
else
	echo "Generate image..."
	if [ $# -ne 1 ]; then
		echo "Usage: $0 <img_name>"
		exit
	fi
i

echo "PWD:$PWD"

mk_image()
{
	local img_path=$1
	local img_dev_name=$2

	local image_table=(
		# name   dir			file			offset
		#"gpt	 u-boot                  mbr-gpt.bin       	0"
		"spl     output                  dfd-u-boot-spl.bin     64"
		"uboot   output                  u-boot.itb        	1024"
		"kernel  output                  linux.itb          	2048"
		"rootfs  buildroot/output/images rootfs.ext4   		40960"
	)
	# offset unit is block, 512Byte

	echo $img_dev_name | grep "dev"
	if [ $? -ne 0 ] && [ -d $img_dev_name ]; then
		echo "Generate the overall image."
		dd if=/dev/zero of=$img_dev_name bs=1M count=1 2> /dev/null
	fi

	for image in "${image_table[@]}"
	do
		img_line=($image)
		img_name=${img_line[0]}
		img_dirs=${img_line[1]}
		img_file=${img_line[2]}
		img_ofst=${img_line[3]}

		if [ -d .repo ]; then
			img_file=${img_path}/${img_dirs}/${img_file}
		else
			img_file=${img_path}/${img_file}
		fi

		if [ ! -z $IMG_CELL ] && [ x"$IMG_CELL" != x"$img_name" ]; then
			continue
		fi

		if [ ! -f $img_file ];then
			echo "$img_file not exist!"
			exit
		fi

		echo " Write $img_name ..."
		if [ -b $img_dev_name ]; then
			SUDO=sudo
		else
			SUDO=
		fi
		set -x
		$SUDO dd if=$img_file of=$img_dev_name seek=$img_ofst
		set +x
	done
}

mk_image $PWD $IMG_DEV_NAME
set -x
sync
sync
sync
set +x
sleep 1
echo "Done"

