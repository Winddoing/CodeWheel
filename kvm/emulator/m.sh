#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年09月27日 星期日 21时29分19秒
# Description	:
##########################################################

as -32 bin.S -o bin.o

ld -m elf_i386 --oformat binary -N -e _start -Ttext 0x10000 -o bin.bin bin.o

hexdump -C bin.bin


gcc -g demo.c -o demo.out

./demo.out
