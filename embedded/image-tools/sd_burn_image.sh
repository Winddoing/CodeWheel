#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : sd_burn_image.sh
#  Author       : wqshao
#  Created Time : 2022-06-23 16:42:28
#  Description  :
##########################################################

PWD=`pwd`
SD_DEV=$1
IMG_CELL=$2

[ ${UID} -ne 0 ] && echo "Please run with sudo" && exit -1

if [ $# -lt 1 ]; then
    echo "Usage: $0 <device>"
    echo "example: "
	echo "	$0 /dev/sdb					#burn all images"
	echo "	$0 /dev/sdb [gpt|spl|uboot|kernel|rootfs]	#Burn a separate image"
    exit
fi

if [ ! -e $SD_DEV ];then
    echo "The USB card reader $SD_DEV is not present!"
    exit
fi

sudo fdisk -l $SD_DEV &> /dev/null
if [ $? -ne 0 ]; then
    echo "The sd card $SD_DEV is not present!"
    exit
fi

echo "PWD:$PWD"

image_table=(
	# name   dir						file            offset
	"gpt	 u-boot						mbr-gpt.bin     0"
	"spl     u-boot						spl.img         64"
	"uboot   u-boot						u-boot.itb      1088"
	"kernel  kernel						boot.img        4096"
	"rootfs  buildroot/output/images	rootfs.squashfs 20480"
)
# offset unit is block, 512Byte

for image in "${image_table[@]}"
do
	img_line=($image)
	img_name=${img_line[0]}
	img_dirs=${img_line[1]}
	img_file=${img_line[2]}
	img_ofst=${img_line[3]}

	if [ -d .repo ]; then
		img_file=${PWD}/${img_dirs}/${img_file}
	else
		img_file=${PWD}/${img_file}
	fi

	if [ ! -z $IMG_CELL ] && [ x"$IMG_CELL" != x"$img_name" ]; then
		continue
	fi

	if [ ! -f $img_file ];then
		echo "$img_file not exist!"
		exit
	fi

	echo " Write $img_name ..."
	set -x
	sudo dd if=$img_file of=$SD_DEV seek=$img_ofst
	set +x
done
set -x
sync
sync
set +x
echo "Done"

