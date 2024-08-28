##########################################################
# File Name		: a.sh
# Author		: winddoing
# Created Time	: 2019年03月04日 星期一 10时45分05秒
# Description	:
##########################################################
#!/bin/bash

set -x
aarch64-linux-gnu-gcc -static -O3 matrix_4x4_mul.c -o matrix_4x4_mul.out
aarch64-linux-gnu-gcc -static -O3 matrix_8x8_mul.c -o matrix_8x8_mul.out
aarch64-linux-gnu-gcc -static -O3 byte_order_rev.c -o byte_order_rev.out
set +x
