#!/bin/sh
PWD="/wqshao"

TOP_PWD="$PWD/msc_test"
MSC_DEV_NODE="/dev/mmcblk0p7"
COUNT_TXT="$TOP_PWD/count.txt"
LOG_FAIL="$TOP_PWD/msc_fail.log"
LOG_PASS="$TOP_PWD/msc_pass.log"

CURRTIME=`date`

echo  "*********************************************"
echo  ""
echo  ""
echo  "reboot" >> $COUNT_TXT
wc -l $COUNT_TXT
echo  ""
echo  ""
echo  "*********************************************"

COUNT=`wc -l $COUNT_TXT`
if [ -b $MSC_DEV_NODE ]; then
	echo "$COUNT : $CURRTIME : mmc init ok"
	echo "$COUNT : $CURRTIME : mmc init ok" >> $LOG_PASS
else
	echo "$COUNT : $CURRTIME : mmc init fail"
	echo "$COUNT : $CURRTIME : mmc init fail" >> $LOG_FAIL
fi

reboot

exit
