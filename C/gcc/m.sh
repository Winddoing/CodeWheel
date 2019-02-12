##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年02月02日 星期六 10时48分07秒
# Description	:
##########################################################
#!/bin/bash

gcc -g -D_GNU_SOURCE -finstrument-functions instrfunc.c -o instrfunc.out -ldl -lpthread -Wl,--export-dynamic
