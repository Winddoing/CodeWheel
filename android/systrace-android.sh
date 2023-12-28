#!/bin/bash
##########################################################
# File Name		: systrace-androoid.sh
# Author		: winddoing
# Created Time	: 2021年01月23日 星期六 14时45分41秒
# Description	:
#      https://dl.google.com/android/repository/platform-tools_r30.0.5-linux.zip
##########################################################

set -x

ip="192.168.101.149"
vm=24
port=$((550 + vm))

TRACE_BUFFER_SZ_KB=16384 #16MB
TRACE_FILE="/data/local/tmp/trace_$port.log"

adb disconnect
adb connect $ip:$port
sleep 0.5
adb root

adb connect $ip:$port
sleep 0.5

# adb shell atrace --list_categories
tracedump=$(echo `adb shell atrace --list_categories | awk '{print $1}'` | sed 's/\n//g')

adb shell "atrace -b $TRACE_BUFFER_SZ_KB -t 30 -z $tracedump > $TRACE_FILE"
adb pull $TRACE_FILE

python platform-tools/systrace/systrace.py  -b $TRACE_BUFFER_SZ_KB --from-file trace_$port.log -o trace_$port.html
