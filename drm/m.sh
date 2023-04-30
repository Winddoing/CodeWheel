#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2021年04月07日 星期三 22时12分26秒
# Description	:
##########################################################

set -x

gcc test.c `pkg-config --libs --cflags libdrm`
