#!/bin/bash
##########################################################
# File Name		: boot_qemu.sh
# Author		: winddoing
# Created Time	: 2022年03月19日 星期六 14时22分42秒
# Description	:
##########################################################


set -x

qemu-system-aarch64 -machine raspi3 -m 256M  -kernel test.elf -S -s
