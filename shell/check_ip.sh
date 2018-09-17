##########################################################
# File Name		: ip.sh
# Author		: wqshao
# Created Time	: 2018年09月17日 星期一 11时47分27秒
# Description	:
##########################################################
#!/bin/bash

up=0
down=0

for siteip in $(seq 1 255)
do
	#site="192.168.2.${siteip}"
	site="172.16.189.${siteip}"
	ping -c1 -W1 ${site} &> /dev/null
	if [ "$?" == "0" ]; then
		up=$[$up+1]
		echo "$site is UP, cnt=$up"
	else
		down=$[$down+1]
		echo "$site is DOWN, cnt=$down"
	fi
done

# 除法
alive=`awk 'BEGIN{printf "%.2f\n",('$up'/'$(($up + $down))')}'`

echo "up:$up, down:$down, alive:$alive"
