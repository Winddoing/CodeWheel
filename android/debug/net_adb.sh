##########################################################
# File Name		: net_adb.sh
# Author		: wqshao
# Created Time	: 2017年09月15日 星期五 11时45分45秒
# Description	:
##########################################################

#!/system/bin/sh

HOST_IP="192.168.10.112"
HOST_MASK="255.255.255.0"
HOST_PORT="5555"

busybox ifconfig eth0 down

busybox ifconfig

busybox ifconfig eth0 up

busybox ifconfig

busybox ifconfig eth0 $HOST_IP netmask $HOST_MASK

busybox ifconfig

stop adbd

setprop service.adb.tcp.port $HOST_PORT

echo "=======>host IP: $HOST_IP:$HOST_PORT"
start adbd
