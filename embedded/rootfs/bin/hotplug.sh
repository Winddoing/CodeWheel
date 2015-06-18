#!/bin/sh

case $MDEV in
sda1)
	DEVNAME=udisk
	MOUNTPOINT=/udisk
	;;
mmcblk0p1)
	DEVNAME=sdcard
	MOUNTPOINT=/sdcard
	;;
*)
	exit 0
	;;
esac

case $ACTION in
remove)
	/bin/umount $MOUNTPOINT || true
	rmdir $MOUNTPOINT >/dev/null 2>&1 || true
	;;
*)
	/bin/mkdir $MOUNTPOINT > /dev/null 2>&1 || true
	/bin/mount -o sync -o noatime -o nodiratime -t vfat /dev/$DEVNAME $MOUNTPOINT > /dev/null 2>&1 || true
	;;
esac

exit 0
