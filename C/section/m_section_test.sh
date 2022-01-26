#!/bin/bash
##########################################################
# File Name		: m_section_test.sh
# Author		: winddoing
# Created Time	: 2022年01月26日 星期三 14时30分12秒
# Description	:
##########################################################

gcc -c section_test.c -o section_test.o

# ld --verbose > ldscript.lds #删除文件前后注释部分，并在bbs段前添加myinit

gcc -T ldscript.lds section_test.o -o section_test.out
