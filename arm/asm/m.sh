##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年03月13日 星期三 15时36分07秒
# Description	:
##########################################################
#!/bin/bash

set -x
aarch64-linux-gnu-gcc -c helloworld.c -o helloworld.o

aarch64-linux-gnu-objdump -D helloworld.o > helloworld.s

aarch64-linux-gnu-gcc opcode_asm.c -o opcode_asm.out
aarch64-linux-gnu-objdump -Dz opcode_asm.out  > opcode_asm.s

