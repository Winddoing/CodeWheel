#!/bin/bash
##########################################################
# File Name		: lookbusy.sh
# Author		: winddoing
# Created Time	: 2021年06月15日 星期二 09时50分36秒
# Description	:
##########################################################

wget http://www.devin.com/lookbusy/download/lookbusy-1.4.tar.gz
tar -xzf lookbusy-1.4.tar.gz
cd lookbusy-1.4
./configure
make -j4
sudo make install
cd ../
#rm -rf lookbusy-1.4*
echo
echo 'Done!'
