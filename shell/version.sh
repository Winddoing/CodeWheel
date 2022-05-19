#!/bin/sh

# __UBOOT_HASH=$(git log -1 --format=%h 2> /dev/null)
# __KERNEL_HASH=$(git log -1 --format=%h 2> /dev/null)
# __BUILDROOT_HASH=$(git log -1 --format=%at 2> /dev/null)
# 
# update_version_info()
# {
# 	sed -i "/^UBOOT_COMMIT_HASH/c UBOOT_COMMIT_HASH=$__UBOOT_HASH" ./version.sh
# 	sed -i "/^KERNEL_COMMIT_HASH/c KERNEL_COMMIT_HASH=$__KERNEL_HASH" ./version.sh
# 	sed -i "/^BUILDROOT_COMMIT_HASH/c BUILDROOT_COMMIT_HASH=$__BUILDROOT_HASH" ./version.sh
# }

# git log -1 --format=%h 2> /dev/null
UBOOT_COMMIT_HASH=111
# git log -1 --format=%h 2> /dev/null
KERNEL_COMMIT_HASH=5.13.0-40-generic
# git log -1 --format=%at 2> /dev/null
BUILDROOT_COMMIT_HASH=Fossa


if [ -z $UBOOT_COMMIT_HASH ] || [ -z $KERNEL_COMMIT_HASH ]; then
	echo "Temporary beta version"
	exit 1
fi

ret=0

dmesg | grep "$KERNEL_COMMIT_HASH" > /dev/null
if [ $? -eq 0 ]; then
	echo "#### The kernel is a normal release test version ($KERNEL_COMMIT_HASH)"
	ret=0
else
	echo "XXXX The kernel is abnormal, please confirm manually"
	ret=1
fi


cat /usr/lib/os-release | grep "$BUILDROOT_COMMIT_HASH" > /dev/null
if [ $? -eq 0 ]; then
	echo "#### The rootfs is a normal release test version ($BUILDROOT_COMMIT_HASH)"
	ret=0
else
	echo "XXXX The rootfs is abnormal, please confirm manually"
	cat /usr/lib/os-release
	ret=1
fi

echo "#### Please confirm the uboot version manually: U-Boot 2021.01-00160-g${UBOOT_COMMIT_HASH}-dirty"


exit $ret

