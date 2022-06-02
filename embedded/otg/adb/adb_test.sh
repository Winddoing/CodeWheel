#!/bin/bash


adb devices | grep "device" | grep -v "attached"
if [ $? -ne 0 ]; then
	echo "not adb devices"
	exit 0
fi


cnt=1
while [ 1 ]
do
	sz=$(expr $RANDOM % 20 + 10)
	echo "sz=$sz, count=$cnt"

	dd if=/dev/urandom of=/tmp/5a5a bs=1M count=$sz 2> /dev/null

	set -x
	adb push /tmp/5a5a /tmp
	rm /tmp/5a5a -rf
	adb shell sync
	sleep 3

	adb pull /tmp/5a5a /tmp
	rm /tmp/5a5a -rf
	adb shell rm /tmp/5a5a
	adb shell sync
	sleep 3

	set +x

	cnt=$((cnt + 1))
done


