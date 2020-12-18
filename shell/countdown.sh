#!/bin/bash
##########################################################
# File Name		: countdown.sh
# Author		: winddoing
# Created Time	: 2020年11月09日 星期一 16时45分03秒
# Description	:
##########################################################

countdown()
{
    for i in `seq -w 3 -1 0`
    do
        t="${i}s"
        echo -ne "\b\b$t"
        sleep 1
    done
    echo ""
}

echo -ne "Countdown:  "
countdown 
