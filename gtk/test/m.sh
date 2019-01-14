##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年01月04日 星期五 17时51分21秒
# Description	:
##########################################################
#!/bin/bash

gcc helloword.c  -o helloword.out `pkg-config --cflags --libs gtk+-3.0`
gcc img.c -o img.out  `pkg-config --cflags --libs gtk+-3.0`
