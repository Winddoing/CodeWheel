##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年03月13日 星期三 15时36分07秒
# Description	:
##########################################################
#!/bin/bash

aarch64-linux-gcc -c helloworld.c -o helloworld.o

aarch64-linux-objdump -D helloworld.o > helloworld.s
