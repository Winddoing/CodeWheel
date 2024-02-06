##########################################################
# File Name		: ip.sh
# Author		: wqshao
# Created Time	: 2018年09月17日 星期一 11时47分27秒
# Description	:
##########################################################
#!/bin/bash

up=0
down=0

net_seg="172.16.40"
echo "Check ip: ${net_seg}.x"
for addr in $(seq 1 255)
do
	#site="192.168.2.${addr}"
	site="${net_seg}.${addr}"
	#echo "site=${site}"
	#ping -c1 -W1 ${site} &> /dev/null
	nc -vz -w 1 ${site} 80 &> /dev/null
	if [ "$?" == "0" ]; then
		up=$[$up+1]
		echo -e "\n$site is UP, cnt=$up"
	else
		down=$[$down+1]
		#echo "$site is DOWN, cnt=$down"
	fi
	#echo -n "#"
	echo -n "${addr}-"
done

# 除法
alive=`awk 'BEGIN{printf "%.2f\n",('$up'/'$(($up + $down))')}'`

echo "up:$up, down:$down, alive:$alive"
