#!/bin/bash
##########################################################
# File Name		: run.sh
# Author		: winddoing
# Created Time	: 2020年12月26日 星期六 15时12分13秒
# Description	:
##########################################################

set -x

echo "Test shell+bin"

mkdir tmp

sed -n '1,/^exit 0$/!p' $0 > ./tmp/test.tar.gz

cd tmp
tar zxvf test.tar.gz
bash ./test.sh
cd -

exit 0
