#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年11月03日 星期二 15时48分15秒
# Description	:
##########################################################

set -x

make 

./avg_svc &

sleep 1

./ravg localhost $RANDOM $RANDOM $RANDOM
