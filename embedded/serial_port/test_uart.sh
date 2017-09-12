##########################################################
# File Name		: test_uart.sh
# Author		: wqshao
# Created Time	: 2017年09月08日 星期五 14时31分26秒
# Description	:
##########################################################
#!/bin/bash


md5="5cc5fb7da900a63ed72a599c9ee5b00f"
#md5=$3

send=0
recv=1

trigger=$1
num=1
err_num=0
ok_num=0

if [ $send -eq $1 ]; then
	file=$2
	cp $file /tmp/
	file="/tmp/$2"
else
	file=$(mktemp)
fi

echo "Start test file: <$(ls -lsh $file)>"
echo "    file md5: $md5"
while true
do
	echo "[$num]./serial_test /dev/ttyS1 4000000 $trigger $file"
	./serial_test /dev/ttyS1 4000000 $trigger $file

	md5_file=$(md5sum $file | awk '{print $1}')

	if [ $recv -eq $trigger ]; then
		echo "md5-ok: $md5"
		echo "md5sum: $md5_file"
		if [ "$md5_file"X = "$md5"X ]; then
			ok_num=$(($ok_num + 1))
			echo "transfter OK($ok_num)........."
		else
			err_num=$(($err_num + 1))
			echo "ERROR: md5 error($err_num), file different"
		fi
	fi

	if [ $send -eq $trigger ]; then
		rm $file
		file=$(mktemp)
		echo "Wait ... (Use the next recv)."
		#sleep 20
	fi
	num=$(($num + 1))

	if [ $send -eq $trigger ]; then
		trigger=$recv
	else
		trigger=$send
	fi
done


