#!/bin/bash
##########################################################
# Copyright (C) 2026 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2026-03-06 17:19:30
#  Description  :
##########################################################

set -x

gcc -shared -fPIC -Wall -o libusb_hook.so hook.c -ldl

BR_PATH="$HOME/work/buildroot/output"
#aarch64-none-linux-gnu-gcc -shared -fPIC -Wall -o libusb_hook.so hook.c -ldl -I ${BR_PATH}/build/libusb-1.0.29/libusb/
aarch64-none-linux-gnu-gcc -shared -fPIC -Wall -o libusb_hook.so hook.c -ldl -I /usr/include/libusb-1.0/


## 运行
#export LD_PRELOAD=./libusb_hook.so
#./usb_exclusive.out 0xabcd 0x1234
#
#LD_PRELOAD=./libusb_hook.so ./usb_exclusive.out 0xabcd 0x1234
