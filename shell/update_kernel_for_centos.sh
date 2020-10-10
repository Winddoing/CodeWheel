#!/bin/bash
##########################################################
# File Name		: update_kernel_for_centos.sh
# Author		: winddoing
# Created Time	: 2020年10月10日 星期六 11时29分06秒
# Description	:
##########################################################

set -x
yum -y groups install "Development Tools"

wget https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.14.105.tar.gz

tar zxvf linux-4.14.105.tar.gz

cp /boot/config-`uname -r` ./linux-4.14.105/.config

cd linux-4.14.105

make menuconfig
make oldconfig

make kernelversion
make -j32 all

make modules_install
# ls -lh /lib/modules

make install
# ls -lh /boot

# grub2-mkconfig -o /boot/grub2/grub.cfg

# awk -F\' '$1=="menuentry " {print i++ " : " $2}' /etc/grub2.cfg'
