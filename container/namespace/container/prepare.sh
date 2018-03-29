##########################################################
# File Name		: mini_sys/prepare.sh
# Author		: wqshao
# Created Time	: 2018年03月27日 星期二 14时57分15秒
# Description	:
##########################################################
#!/bin/bash

PWD=$('pwd')
ROOTFS=$PWD/rootfs

for dir in `ls /`
do
	mkdir -p  $ROOTFS/$dir

done

mkdir $ROOTFS/usr/libexec
mkdir $ROOTFS/usr/bin

cp -r /bin/*  $ROOTFS/bin/
cp -r /usr/bin/*  $ROOTFS/usr/bin/

cp -r /lib/* $ROOTFS/lib/

cp -r /etc/* $ROOTFS/etc/

