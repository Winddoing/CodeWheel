##########################################################
# File Name		: a.sh
# Author		: winddoing
# Created Time	: 2019年03月04日 星期一 10时45分05秒
# Description	:
##########################################################
#!/bin/bash

aarch64-linux-gcc matrix_4x4_mul.c -O3 -o matrix_4x4_mul.out
aarch64-linux-gcc matrix_8x8_mul.c -O3 -o matrix_8x8_mul.out
