#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年04月30日 星期二 14时30分43秒
# Description	:
##########################################################

#simd_flag="-mpopcnt -msse -msse2 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx -mavx2 -mbmi"
simd_flag="-mavx2"

nasm x86_64/jfdctint-avx2.asm -o jfdctint-avx2.o -I nasm/  -D__x86_64__  -f elf64 -DPIC
nasm x86_64/jidctint-avx2.asm -o jidctint-avx2.o -I nasm/  -D__x86_64__  -f elf64 -DPIC
nasm x86_64/jfdctfst-sse2.asm -o jfdctfst-sse2.o -I nasm/  -D__x86_64__  -f elf64 -DPIC
nasm x86_64/jidctfst-sse2.asm -o jidctfst-sse2.o -I nasm/  -D__x86_64__  -f elf64 -DPIC

gcc *.o *.c ${simd_flag} -Wall -O3


ctags -R .



