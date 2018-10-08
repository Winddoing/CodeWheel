##########################################################
# File Name		: libpwd.sh
# Author		: wqshao
# Created Time	: 2018年10月08日 星期一 10时55分43秒
# Description	:
##########################################################
#!/bin/bash
PWD=`pwd`

echo "/* Do not Edit */" > libcount.h
echo "#define LIBPATH  \"${PWD}/libcount.so\"" >> libcount.h
