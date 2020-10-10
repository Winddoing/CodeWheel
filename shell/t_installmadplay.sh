#!/bin/sh

SRC_DIR=`pwd`
DST_DIR=/usr/local/src

for FILE in libid3tag libmad madplay
do
	echo -n "Begin to install $FILE, waiting..."
	tar xfz $FILE* -C $DST_DIR > /dev/null
	cd $DST_DIR/$FILE*
	./configure > /dev/null 2>&1
	make > /dev/null 2>&1
	make install > /dev/null 2>&1
	echo " done."
	cd $SRC_DIR
done
echo "我安装完成了,OY！"
echo "开始配置共享库..."
echo "" >> /etc/ld.so.conf
echo "/usr/local/lib" >> /etc/ld.so.conf
ldconfig
echo "完成，您可以利用madplay <mp3文件列表> 播放音乐了!"
unset FILE SRC_DIR DST_DIR

