#!/bin/sh


PWD=`pwd`
# test path
SD_MOUNT=`pwd`
# log save path
LOG_PWD=`pwd`

TIME=`date "+%Y-%m-%d"`
TEST_TIME=`date "+%Y-%m-%d-%Hh-%Mmin"`


if [ $# -lt 3 ]; then
	echo "Usage:    $0 <-t type> <dst device> <test file>"
	echo "example:    $0 -t vfat /dev/mmcblk0  test.tar.bz2"
	exit
fi


TEST_SRC_FILE=$4
TEST_DST_FILE=test_bak.tar.bz2
TEST_LOG_FILE=$LOG_PWD/$TIME-copy-test.log

if [ ! -d $SD_MOUNT ];then
	mkdir -p $SD_MOUNT
fi

if [ ! -d $LOG_PWD ];then
	mkdir -p $LOG_PWD
fi


#mount -t $2 $3 $SD_MOUNT

echo "============== Test Start Time: $TEST_TIME " >> $TEST_LOG_FILE
echo "==== Test command: mount -t $2 $3 $SD_MOUNT " >> $TEST_LOG_FILE
echo "==== Test dir : " >> $TEST_LOG_FILE
echo "====      PWD : $PWD" >> $TEST_LOG_FILE
echo "==== SD_MOUNT : $SD_MOUNT " >> $TEST_LOG_FILE
echo "====  LOG_PWD : $LOG_PWD" >> $TEST_LOG_FILE
echo "===================================================" >> $TEST_LOG_FILE

COUNT=0
PASS_TO_SD_COUNT=0
FAIL_TO_SD_COUNT=0

PASS_TO_NFS_COUNT=0
FAIL_TO_NFS_COUNT=0

while true;do
echo "======test $COUNT ======="
if [ -f "$PWD/$TEST_SRC_FILE" ];then
	echo "===========>>>Test count: $COUNT" >> $TEST_LOG_FILE
	echo "	copy to  SD: PASS($PASS_TO_SD_COUNT), FAIL($FAIL_TO_SD_COUNT)" >> $TEST_LOG_FILE
	echo "	copy to NFS: PASS($PASS_TO_NFS_COUNT), FAIL($FAIL_TO_NFS_COUNT)" >> $TEST_LOG_FILE
	md5sum $PWD/$TEST_SRC_FILE > $SD_MOUNT/test.md5
	cp $PWD/$TEST_SRC_FILE $SD_MOUNT/$TEST_DST_FILE
	sync
	md5sum $SD_MOUNT/$TEST_DST_FILE >> $SD_MOUNT/test.md5
	#md5sum -c -s $SD_MOUNT/test.md5
	md5sum -c  $SD_MOUNT/test.md5
	if [ $? -ne 0 ];then
		echo "$COUNT test md5sum prepare fail!!"
		echo "==========START=========>>$COUNT test md5sum prepare fail" >> $TEST_LOG_FILE
		exit
	fi
	echo "$COUNT test md5sum prepare pass!!"
	echo "==========START=========>>$COUNT test md5sum prepare pass" >> $TEST_LOG_FILE
	rm $SD_MOUNT/$TEST_DST_FILE
	echo "#####rm $SD_MOUNT/$TEST_DST_FILE ok#####"
	cp $PWD/$TEST_SRC_FILE $SD_MOUNT/$TEST_DST_FILE
	echo "****cp $PWD/$TEST_SRC_FILE ->$SD_MOUNT/$TEST_DST_FILE ok****"
	#md5sum -c -s $SD_MOUNT/test.md5
	md5sum -c $SD_MOUNT/test.md5
	if [ $? -ne 0 ];then
		FAIL_TO_SD_COUNT=`expr $FAIL_TO_SD_COUNT + 1`
		echo "[FAIL] test $COUNT md5sum test NFS --> SD fail!!"
		echo "[FAIL] test $COUNT md5sum test NFS --> SD fail!!" >> $TEST_LOG_FILE
		exit
	else
		PASS_TO_SD_COUNT=`expr $PASS_TO_SD_COUNT + 1`
		echo "[PASS] test $COUNT md5sum test NFS --> SD pass!!"
		echo "[PASS] test $COUNT md5sum test NFS --> SD pass!!" >> $TEST_LOG_FILE
	fi
	rm $PWD/$TEST_SRC_FILE
	echo "#### rm $PWD/$TEST_SRC_FILE ok####"
fi
echo "$PWD/$TEST_SRC_FILE file not find ..."
cp $SD_MOUNT/$TEST_DST_FILE $PWD/$TEST_SRC_FILE
sync
#md5sum -c -s $SD_MOUNT/test.md5
md5sum -c  $SD_MOUNT/test.md5
if [ $? -ne 0 ];then
	FAIL_TO_NFS_COUNT=`expr $FAIL_TO_NFS_COUNT + 1`
	echo "[FAIL] test $COUNT md5sum test SD --> NFS fail!!"
	echo "[FAIL] test $COUNT md5sum test SD --> NFS fail!!" >> $TEST_LOG_FILE
#	exit
else
	PASS_TO_NFS_COUNT=`expr $PASS_TO_NFS_COUNT + 1`
	echo "[PASS] test $COUNT md5sum test SD --> NFS pass!!"
	echo "[PASS] test $COUNT md5sum test SD --> NFS pass!!" >> $TEST_LOG_FILE
fi
rm $SD_MOUNT/$TEST_DST_FILE
rm $SD_MOUNT/test.md5
echo "#####rm $SD_MOUNT/$TEST_DST_FILE $SD_MOUNT/test.md5 ok..........."
COUNT=`expr $COUNT + 1`
done


