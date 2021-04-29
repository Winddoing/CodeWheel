#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2021年04月29日 星期四 16时36分39秒
# Description	:
##########################################################

set -x

gcc test.c  -g -rdynamic -O0
