##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年01月25日 星期五 10时33分50秒
# Description	:
##########################################################
#!/bin/bash

gcc test.c -o test.out -lGL -lglut

gcc yuv_play.c -o yuv_play.out -lGL -lglut

gcc demo1.c -o demo1.out -lGL -lglut
