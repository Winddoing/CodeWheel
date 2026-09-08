#!/bin/bash
##########################################################
# Copyright (C) 2026 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2026-03-06 17:19:30
#  Description  :
##########################################################

set -x

gcc usb_only_reset.c `pkgconf --libs libusb-1.0`
gcc usb_exclusive.c `pkgconf --libs libusb-1.0`

BR_PATH="$HOME/work/buildroot/output"
aarch64-none-linux-gnu-gcc usb_only_reset.c -o usb_only_reset.out -L ${BR_PATH}/target/usr/lib/ -lusb-1.0 -I ${BR_PATH}/build/libusb-1.0.29/libusb/ -L ${BR_PATH}/target/lib/ -ludev
aarch64-none-linux-gnu-gcc usb_exclusive.c -o usb_exclusive.out -L ${BR_PATH}/target/usr/lib/ -lusb-1.0 -I ${BR_PATH}/build/libusb-1.0.29/libusb/ -L ${BR_PATH}/target/lib/ -ludev
