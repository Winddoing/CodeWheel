#!/bin/bash
##########################################################
# Copyright (C) 2026 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2026-03-13 13:46:31
#  Description  :
##########################################################

set -x

#aarch64-linux-gnu-gcc -S -O2 smp_boot.c -o smp_boot.S -nostdlib -ffreestanding -fomit-frame-pointer -fverbose-asm
#aarch64-linux-gnu-gcc -S -O2 smp_boot.c -o smp_boot.S -fno-toplevel-reorder -fno-schedule-insns -fno-schedule-insns2
aarch64-linux-gnu-gcc -S smp_boot.c -o smp_boot_1.S
aarch64-linux-gnu-gcc -c smp_boot.c -o smp_boot.o
aarch64-linux-gnu-objdump -d smp_boot.o > smp_boot_2.S

aarch64-linux-gnu-gcc -c smp_boot_asm.s -o smp_boot_asm.o
aarch64-linux-gnu-objdump -d smp_boot_asm.o > smp_boot_asm.S

aarch64-linux-gnu-gcc -c smp_boot_asm1.s -o smp_boot_asm1.o
aarch64-linux-gnu-objdump -d smp_boot_asm1.o > smp_boot_asm1.S
