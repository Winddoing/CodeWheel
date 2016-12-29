#!/bin/sh

mips-linux-gnu-gcc prefetch-loop-arrays.c  -c -fprefetch-loop-arrays -O3 -o aa.o

mips-linux-gnu-objdump -d aa.o > prefetch-loop-arrays.s

mips-linux-gnu-gcc prefetch-loop-arrays.c  -c -O3 -o bb.o

mips-linux-gnu-objdump -d bb.o > no_prefetch-loop-arrays.s

rm aa.o bb.o
