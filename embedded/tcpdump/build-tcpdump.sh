##########################################################
# File Name		: tcpdump-build.sh
# Author		: wqshao
# Created Time	: 2018年08月15日 星期三 16时04分40秒
# Description	:
##########################################################
#!/bin/bash

# host

# for libpcap error: configure: error: Neither flex nor lex was found.
#sudo apt install flex bison


PWD=$(pwd)

TCPDUMP="tcpdump-4.9.2"
LIBPCAP="libpcap-1.9.0"

export CC=arm-linux-gnueabihf-gcc

# http://www.tcpdump.org

for software in ${TCPDUMP} ${LIBPCAP}
do
	echo "Download $software ..."
	echo "wget http://www.tcpdump.org/release/${software}.tar.gz"
	#wget http://www.tcpdump.org/release/${software}.tar.gz
	echo "tar xvf ${software}.tar.gz"
	tar xvf ${software}.tar.gz
done

cd ${LIBPCAP}
./configure --host=arm-linux --with-pcap=linux --prefix=${PWD}/out
make; make install
cd -

cd ${TCPDUMP}
./configure --host=arm-linux --with-system-libpcap=${PWD}/../${LIBPCAP}/out/lib --prefix=${PWD}/out
make; make install
cd -

cp ${PWD}/${TCPDUMP}/out/sbin/tcpdump .

