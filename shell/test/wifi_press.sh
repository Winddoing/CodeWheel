#!/bin/sh
PWD=`pwd`
TIME=`date "+%Y-%m-%d"`
TEST_TIME=`date "+%Y-%m-%d-%Hh-%Mmin"`


URL='http://192.168.4.6/mediawiki/images/2/21/Wifi_download_test.ps'

myfile="$PWD/Wifi_download_test.ps"
myfile_old="$PWD/Wifi_download_test.ps_old"

LOG_FILE="$PWD/$TIME-wifi_test.log"

rm $myfile $myfile_old

wget $URL

mv Wifi_download_test.ps Wifi_download_test.ps_old


let "COUNT=0"
let "OK_COUNT=0"
let "ERR_COUNT=0"

echo "============== Test Start Time: $TEST_TIME ============" >> $LOG_FILE
while true;do
	echo "---begin---download----------"
	echo "===========>>>Test count: $COUNT, PASS($OK_COUNT), FAIL($ERR_COUNT)" >> $LOG_FILE

	wget $URL

	while [ $? -ne 0 ]; do
		sleep 3
		wget $URL
	done

	if [ ! -f "$myfile" ];then
		echo "---download--update11.zip--error--"
		echo "[==ERROR==]: download error!!!" >> $LOG_FILE
		exit
	else
		echo "---download--update11.zip---ok--"
		echo "[==PSAA==]: download ok!!!" >> $LOG_FILE
	fi

	#diff $myfile  $myfile_old  >> ./wifi_diff_log.txt
	diff $myfile  $myfile_old

	if [ $? -eq 0 ];then
		let "OK_COUNT=$OK_COUNT+1"
		echo "==========download success=============="
		echo ""
		echo "OK_count = $OK_COUNT"
		echo "[**PASS**]: ($OK_COUNT)Download file same ...." >> $LOG_FILE
		echo ""
		echo "======================================"
	else
		let "ERR_COUNT=$ERR_COUNT+1"
		echo "==========download fail=============="
		echo ""
		echo "ERR_count = $ERR_COUNT"
		echo "[**ERROR**]: ($ERR_COUNT)Download file diff error ...." >> $LOG_FILE
		echo ""
		echo "======================================"
	fi

	let "COUNT=$COUNT+1"
	echo "--test--$COUNT--"
	rm $myfile
	echo "--rm----info--ok "
done
