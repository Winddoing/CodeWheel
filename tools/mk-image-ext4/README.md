
## 制作分区

>parted命令是由GNU组织开发的一款功能强大的磁盘分区和分区大小调整工具

``` shell
dd if=/dev/zero of=${SYSTEM} bs=1M count=0 seek=$SIZE

# 创建新的分区表
parted -s ${SYSTEM} mklabel gpt
parted -s ${SYSTEM} unit s mkpart loader1 ${LOADER1_START} $(expr ${RESERVED1_START} - 1)
parted -s ${SYSTEM} unit s mkpart reserved1 ${RESERVED1_START} $(expr ${RESERVED2_START} - 1)
parted -s ${SYSTEM} unit s mkpart reserved2 ${RESERVED2_START} $(expr ${LOADER2_START} - 1)
parted -s ${SYSTEM} unit s mkpart loader2 ${LOADER2_START} $(expr ${ATF_START} - 1)
parted -s ${SYSTEM} unit s mkpart atf ${ATF_START} $(expr ${BOOT_START} - 1)
parted -s ${SYSTEM} unit s mkpart boot ${BOOT_START} $(expr ${ROOTFS_START} - 1)
parted -s ${SYSTEM} set 6 boot on
parted -s ${SYSTEM} unit s mkpart root ${ROOTFS_START} 100%
```

> - `-s`: 不提示用户



