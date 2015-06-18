###yaffs文件系统烧写
        nand write.yaffs 0x30008000 0x600000 0x__size___

size:表示文件系统镜像的真实大小，使用tftp上传大小多大，写多大。