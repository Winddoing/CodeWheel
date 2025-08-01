##########################################################
# File Name		: ip.sh
# Author		: wqshao
# Created Time	: 2018年09月17日 星期一 11时47分27秒
# Description	:
##########################################################
#!/bin/bash

net_seg="172.16.40"
echo -e "Check ip: ${net_seg}.x\n"

echo "-------------------------"
printf " IP address\tstatus\n"
for addr in $(seq 1 255)
do
	# 多线程
	(
	#site="192.168.2.${addr}"
	site="${net_seg}.${addr}"
	#echo "site=${site}"
	#ping -c1 -W1 ${site} &> /dev/null
	nc -vz -w 1 ${site} 22 &> /dev/null
	if [ "$?" == "0" ]; then
		printf "%s.%-3d\talive   http://%s.%-3d\n" ${net_seg} ${addr} ${net_seg} ${addr}
		#echo -e "$site is alive."
	fi
	#echo -n "${addr}-"
	)&
done

wait

echo "-------------------------"

#arp -a
