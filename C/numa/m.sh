#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年12月09日 星期三 11时34分45秒
# Description	:
##########################################################



gcc test.c -o test.out -lnuma

gcc test_numa.c -o test_numa.out -lnuma
