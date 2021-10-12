#!/bin/bash
##########################################################
# File Name		: autopart.sh
# Author		: winddoing
# Created Time	: 2021年10月12日 星期二 14时13分12秒
# Description	: 将SD卡设置为GPT分区格式,并分三个区为boot,rootfs,data
#
# For example (default configuration):
#	Device     Start  End     Sectors  Size Type
# /dev/sda1    4096   20479   16384    8M   EFI System
# /dev/sda2    20480  69631   49152    24M  Linux filesystem
# /dev/sda3    69632  1984511 1914880  935M Linux filesystem
##########################################################

sd_dev=$1
part_type=gpt
unit="MiB"
free_space=2
boot_part=8
rootfs_part=24
data_part=100%  #剩余所有空间

if [ $# -lt 1 ]; then
	echo "Usage: $0 <device>"
	echo "example: $0 /dev/sdb"
	exit
fi

which parted > /dev/null
if [ $? -ne 0 ]; then
	sudo apt install parted
fi
which fdisk > /dev/null
if [ $? -ne 0 ]; then
	sudo apt install fdisk
fi

sudo parted ${sd_dev} print | grep SD > /dev/null
if [ $? -ne 0 ]; then
	echo "the current device($sd_dev) is not an SD card"
	exit
fi

set -x

echo "Delete old partition table ..."
sudo dd if=/dev/zero of=/dev/sda bs=1M count=1 seek=0

echo "Create a GPT disklabel (partition table)"
yes | sudo parted ${sd_dev} mklabel ${part_type}
sudo parted ${sd_dev} quit

echo "Create boot partition !"
# 1 boot
pstart=$((0 + free_space))
pend=$((pstart + boot_part))
sudo parted ${sd_dev} mkpart boot ext4 ${pstart}${unit} ${pend}${unit}
sudo parted ${sd_dev} set 1 esp on
sudo parted ${sd_dev} quit

# 2 rootfs
echo "Create rootfs partition !"
pstart=$pend
pend=$((pstart + rootfs_part))
sudo parted ${sd_dev} mkpart rootfs ext4 ${pstart}${unit} ${pend}${unit}
sudo parted ${sd_dev} quit

# 3 data
echo "Create data partition !"
pstart=$pend
pend=${data_part}
sudo parted ${sd_dev} mkpart data ext4 ${pstart}${unit} ${pend}${unit}
sudo parted ${sd_dev} quit
echo "Format the data partition(${sd_dev}3) as ext4"
yes | sudo mkfs.ext4 ${sd_dev}3

sudo parted ${sd_dev} print
sudo fdisk -l ${sd_dev}
