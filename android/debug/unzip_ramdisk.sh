#!/bin/bash

BAK_RAMDISK=ramdisk-bak.img
UNZIP_RAMDISK="$1.gz"

echo "unzip ramdisk"
echo "$#"
if [ $# -ne 1 ]; then
	echo "$0 ramdisk.img"
	exit 1
fi

echo "Rename ramdisk ..."

cp $1 $BAK_RAMDISK
cp $1 $UNZIP_RAMDISK

echo "gunzip $UNZIP_RAMDISK"

gunzip $UNZIP_RAMDISK

echo "mkdri root .."

mkdir root

cd root
echo "cpio ramdisk.img ..."
echo "`pwd`"
cpio -i -F ../$1

cd ..

echo "exit ..."
exit 0
