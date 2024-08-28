#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : mk_upgrade_img.sh
#  Author       : wqshao
#  Created Time : 2022-06-30 16:10:26
#  Description  :
##########################################################

mk_image()
{
	local img_path=$1
	local img_dev_name=$2

	local image_table=(
		# name   dir					 file              offset
		"spl	 u-boot                  spl-with-gpt.img  0"
		#"spl     u-boot                  spl.img           64"
		"uboot   u-boot                  u-boot.itb        1088"
		"kernel  kernel                  boot.img          5184"
		"rootfs  buildroot/output/images rootfs.squashfs   21568"
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

		if [ ! -f $img_file ];then
			echo "$img_file not exist!"
			exit
		fi

		echo " Write $img_name ..."
		set -x
		dd if=$img_file of=$img_dev_name seek=$img_ofst
		set +x
	done
}

gen_upgrabe_file()
{
	local img_dir=$1
	local vx_img=$img_dir/$2
	local dev_type=$3

	local NET_UPGRADE_DIR="$img_dir/net_upgrade_$3"
	local UPGRADE_FILE_VERSION="version.txt"
	local UPGRADE_FILE_HEAD_SZ=512
	local UPGRADE_FILE_VXIMAGE="net_upgrade_512crc.bin"

	local img_version="vx_images_r_v0.0.1_stub"

	if [ ! -d $NET_UPGRADE_DIR ]; then
		mkdir $NET_UPGRADE_DIR
	fi

	cd $NET_UPGRADE_DIR
	set -x
	# Generate CRC header
	cksum $vx_img | xxd -r -p > crc.bin
	dd if=/dev/zero of=file.bin bs=$UPGRADE_FILE_HEAD_SZ count=1
	cat crc.bin file.bin > file1.bin
	dd if=file1.bin of=crc.bin bs=$UPGRADE_FILE_HEAD_SZ count=1

	# Mirror merge
	cat crc.bin $vx_img > $UPGRADE_FILE_VXIMAGE

	rm crc.bin file1.bin file.bin

	# Generate version info
	echo $img_version > $UPGRADE_FILE_VERSION
	echo "format_data_partition_en" >> $UPGRADE_FILE_VERSION
	if [ x"$dev_type" = x"mtd" ]; then
		echo "mtd_dev" >> $UPGRADE_FILE_VERSION
	else
		echo "mmc_dev" >> $UPGRADE_FILE_VERSION
	fi

	# Generate md5sum.txt
	md5sum $UPGRADE_FILE_VXIMAGE $UPGRADE_FILE_VERSION > md5sum.txt
	set +x
	cd -
}

mk_image $PWD vx_t.bin

gen_upgrabe_file $PWD vx_t.bin mmc
