#!/bin/sh

echo "Install madplay"

SRC=/usr/local/src

for FILE in libid3tag libmad madplay
do
	cd /work/software
	echo "Install $FILE....."

	tar zxvf $FILE* -C $SRC > /dev/null 2>&1

	cd $SRC/$FILE*

		./configure > /dev/null 2>&1
		make > /dev/null 2>&1
		make install > /dev/null 2>&1
done

echo "finish....."

cd $SRC/madplay*

ldd madplay

echo "add libxxxx...."

echo "/usr/local/lib" >> /etc/ld.so.conf
ldconfig

ldd madplay

