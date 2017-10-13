##########################################################
# File Name		: m.sh
# Author		: wqshao
# Created Time	: 2017年10月13日 星期五 16时37分42秒
# Description	:
##########################################################
#!/bin/bash

mips-linux-gnu-gcc test.c -S -o test.s
echo "mips-linux-gnu-gcc test.c -S -o test.s"
mips-linux-gnu-gcc test.s -s -o test.o
echo "mips-linux-gnu-gcc test.s -s -o test.o"
mips-linux-gnu-objdump -D test.o > test.S
echo "mips-linux-gnu-objdump -D test.o > test.S"

