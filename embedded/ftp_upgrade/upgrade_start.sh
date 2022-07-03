#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : upgrade_start.sh
#  Author       : wqshao
#  Created Time : 2022-06-29 14:06:33
#  Description  :
##########################################################

#UPGRADE_SRV_IP="152.173.123.1"
UPGRADE_SRV_IP="172.16.40.118"

#########################################################
# Must be consistent with the image generation parameters
UPGRADE_FILE_VERSION="version.txt"
UPGRADE_FILE_HEAD_SZ=512
UPGRADE_FILE_VXIMAGE="net_upgrade_512crc.bin"
#########################################################

UPGRADE_DEV="/dev/mmcblk0"
UPGRADE_IMG="net_upgrade.bin"
MMC_DATA_PARTITION="/dev/mmcblk0p3"
MTD_DATA_PARTITION="/dev/mtd3" #TODO: stub

cd /tmp/

tools_check()
{
	echo "Tool check"
	local tool_cmd=("ftpget xxd cksum diff dd")
	for cmd in "${tool_cmd[@]}"
	do
		which $cmd
		if [ $? -ne 0 ]; then
			echo "[xxx] $cmd command not found. Please check the file system."
			exit 1
		fi
	done
}

download()
{
	local file=$1

	for retry in `seq 3`
	do
		echo Download $file, retry=$retry
		[ -f $file ] && rm $file
		#ftpget $UPGRADE_SRV_IP $file /UDISK/$file
		ftpget $UPGRADE_SRV_IP $file -o $file
		if [ $? -eq 0 ]; then
			echo "Download ok."
			return 0
		fi
	done

	return 11
}

analyze_version()
{
	local remote_version=$(cat $UPGRADE_FILE_VERSION | awk 'NR==1' | awk '$1=$1')
	local data_format=$(cat $UPGRADE_FILE_VERSION | awk 'NR==2' | awk '$1=$1')
	local upgrade_dev=$(cat $UPGRADE_FILE_VERSION | awk 'NR==3' | awk '$1=$1')

	local local_version="v0.0.1"
	#TODO: version check, Prevent repeated upgrades
	echo "version file, 1 line: [$remote_version], local [$local_version]"
	if [ x"$local_version" = x"$remote_version" ]; then
		echo "Check the version is the same."
		return 1
	fi

	echo "version file, 2 line: $data_format"
	if [ x"$data_format" = x"format_data_partition_en" ]; then
		local mount_pt="/root"
		if [ x"$upgrade_dev" = x"mtd_dev" ]; then
			mount_pt=$(cat /etc/fstab  | grep $MTD_DATA_PARTITION | awk '{print $2}')
		else
			mount_pt=$(cat /etc/fstab  | grep $MMC_DATA_PARTITION | awk '{print $2}')
		fi

		echo "Mount point: $mount_pt"
		touch $mount_pt/en_format_data_partition
	fi

	echo "version file, 3 line: $upgrade_dev"
	if [ x"$upgrade_dev" = x"mtd_dev" ]; then
		UPGRADE_DEV="/dev/mtd0"
	else
		UPGRADE_DEV="/dev/mmcblk0"
	fi
	echo "Upgrade dev: $upgrade_dev, [$UPGRADE_DEV]"

	return 0
}

verify_upgrade_img()
{
	# Image format:
	# +----------+---------------------------+
	# |   CRC    |       vximage             |
	# +----------+---------------------------+
	#    512bit
	set -x
	dd if=$UPGRADE_FILE_VXIMAGE of=crc.old bs=$UPGRADE_FILE_HEAD_SZ count=1
	dd if=$UPGRADE_FILE_VXIMAGE of=$UPGRADE_IMG bs=$UPGRADE_FILE_HEAD_SZ skip=1

	cksum $UPGRADE_IMG | xxd -r -p > crc.new
	dd if=/dev/zero of=file.bin bs=$UPGRADE_FILE_HEAD_SZ count=1
	cat crc.new file.bin > file1.bin
	dd if=file1.bin of=crc.new bs=$UPGRADE_FILE_HEAD_SZ count=1
	rm file.bin file1.bin

	set +x

	diff crc.old crc.new
	if [ $? -ne 0 ]; then
		echo "[xxx] upgrade image error."
		return 1
	fi

	echo "The upgrade image verification is correct."

	return 0
}

set_led_status()
{
	echo "Set led status"
	#TODO
	# while [ 1 ]
}

start_upgrade()
{
	echo "Start upgrade "

	set -x
	dd if=$UPGRADE_IMG of=$UPGRADE_DEV conv=sync
	set +x
}

vx_upgrate()
{
	echo "VX upgrade ..."
	local start_time=$(date +"%s")
	download $UPGRADE_FILE_VERSION
	if [ $? -ne 0 ]; then
		echo "[xxx] Download $UPGRADE_FILE_VERSION file failed, exit the upgrade process."
		exit 1
	fi

	analyze_version
	if [ $? -ne 0 ]; then
		echo "$UPGRADE_FILE_VERSION file parsing same version, no need to do UPGRADE."
		exit 0
	fi

	download $UPGRADE_FILE_VXIMAGE
	if [ $? -ne 0 ]; then
		echo "[xxx] Download $UPGRADE_FILE_VXIMAGE file failed, exit the upgrade process."
		exit 1
	fi

	verify_upgrade_img
	if [ $? -ne 0 ]; then
		echo "[xxx] $UPGRADE_FILE_VXIMAGE verify failed, exit the upgrade process."
		exit 1
	fi

	set_led_status
	start_upgrade
	sync
	sleep 1
	local end_time=$(date +"%s")
	echo "Upgrade finishing(time $((end_time - start_time))s), reboot"
	reboot
}

#main
tools_check
vx_upgrate
