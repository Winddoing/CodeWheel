##########################################################
# File Name		: arm-iperf.sh
# Author		: wqshao
# Created Time	: 2018年07月26日 星期四 17时53分13秒
# Description	:
##########################################################
#!/bin/bash

IPERF_VERSION="3.1.3"
ARM_GCC="arm-linux-gnueabihf-gcc"
PWD=`pwd`

wget https://iperf.fr/download/source/iperf-${IPERF_VERSION}-source.tar.gz
tar zxvf iperf-${IPERF_VERSION}-source.tar.gz
cd iperf-${IPERF_VERSION}/
mkdir install
./configure  --host=arm-linux CC=${ARM_GCC} --prefix=${PWD}/install

make

make install
