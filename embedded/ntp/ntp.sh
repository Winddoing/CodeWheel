##########################################################
# File Name		: ntp.sh
# Author		: wqshao
# Created Time	: 2018年05月21日 星期一 18时40分24秒
# Description	:
##########################################################
#!/bin/bash

wget https://www.eecis.udel.edu/~ntp/ntp_spool/ntp4/ntp-4.2/ntp-4.2.8p11.tar.gz 
                                                                                                 
tar zxvf ntp-4.2.8p11.tar.gz                                                    
                                                                                
cd ntp-4.2.8p11                                                                 

PWD=`pwd`                                                                       
                                                                                
echo "xxxxxxxxxxxx$PWD"                                                         
                                                                                
rm $PWD/install -rf                                                                                                             
mkdir $PWD/install                                                              
                                                                                
echo "./configure --host=arm-linux CC=arm-gcc49-linux-gnueabi-gcc --prefix=$PWD/install/  --with-yielding-select=yes"
./configure --host=arm-linux CC=arm-gcc49-linux-gnueabi-gcc --prefix=$PWD/install/  --with-yielding-select=yes
                                                                                
make                                                                            
                                                                                
make install                                                                    
                                                                                                                                
                                                                                                                                
                                                                                                                                

