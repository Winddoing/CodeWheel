#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年12月26日 星期六 15时20分04秒
# Description	:
##########################################################

tar zcvf test.tar.gz test.sh
cat run.sh test.tar.gz > run-tst.sh

#test

sh run-tst.sh
