##########################################################
# File Name		: m.sh
# Author		: wqshao
# Created Time	: 2018年07月14日 星期六 14时48分46秒
# Description	:
##########################################################
#!/bin/bash

PWD=`pwd`

make

# LD_LIBRARY_PATH环境变量用于在程序加载运行期间查找动态链接库时指定除了系统默认路径之外的其他路径
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD

echo ""
echo "***************** Out Result ***************"
./test.out
