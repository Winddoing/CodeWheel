#!/bin/bash
PWD=`pwd`

DATA_SCR=/dev/urandom

DEVICE="/dev/mmcblk"

MSC_DEV_SEEK=0
DATA_SIZE=1 #1M
DD_BS=1024 #KB
DEBUG=0
TEST_FILE_SIZE=0
MOUNT_POINTER=$PWD/sd_mount

#Default
W_DATA=$PWD/aaa.img
R_DATA=$PWD/bbb.img
TEST_MSC_DEV=/dev/mmcblk0p1
FORMAT="ext4"
TEST_LOG=$PWD/log
TMP_OUT=/dev/null
RESULT_OUT=$TEST_LOG

debug_test ()
{
	TEST_MSC_DEV=$PWD/wqshao_debug

	DEBUG=1
}

# arg : test name,  total tests, pass tests, fail tests
#function test_result ()
test_result ()
{
	local log

	log=$RESULT_OUT/$1_test_result.log

	echo "" | tee -a $log
	echo "  [$1] Test Result:" | tee -a $log
	echo "******************************" | tee -a $log
	echo "**** Test file size: $TEST_FILE_SIZE" | tee -a $log
	echo "****" | tee -a $log
	echo "**** Total test items: $2" | tee -a $log
	echo "**** Pass  test items: $3" | tee -a $log
	echo "**** Fail  test items: $4" | tee -a $log
	echo "******************************" | tee -a $log
	exit 1;
}

test_opt ()
{
	local cnt
	local tmp

	tmp=`expr $DATA_SIZE \* 1024 \* 1024`
	cnt=`expr $tmp / $DD_BS`

	case $1 in
		"random" )
			echo "dd if=$2 of=$3 bs=$DD_BS count=$cnt > $TMP_OUT 2>&1"
			dd if=$2 of=$3 bs=$DD_BS count=$cnt > $TMP_OUT 2>&1
			;;
		"write" )
			echo "dd if=$2 of=$3 seek=$4 bs=$DD_BS count=$cnt conv=fsync > $TMP_OUT 2>&1"
			dd if=$2 of=$3 seek=$4 bs=$DD_BS count=$cnt conv=fsync > $TMP_OUT 2>&1
			;;
		"read" )
			echo "dd if=$2 of=$3 skip=$4 bs=$DD_BS count=$cnt > $TMP_OUT 2>&1"
			dd if=$2 of=$3 skip=$4 bs=$DD_BS count=$cnt > $TMP_OUT 2>&1
			;;
	esac
}

file_diff ()
{
	local file_name
	local tmp

	tmp=$5          #/dev/mmcblk1p3
	tmp=${tmp##*/}  #mmcblk1p3

	file_name="$3error_data_opt_$tmp"_seek_"$4.img"

	echo "diff $1 $2"
	diff $1 $2
	if [ $? -ne 0 ]; then
		echo "[ERROR]: Test data error"
		cp $R_DATA $TEST_LOG/$file_name
		return 1;
	fi

	return 0;
}

test_pre ()
{
	if [ $DEBUG -eq 0 ] && [ ! -b "$TEST_MSC_DEV" ]; then
		echo "Device: $TEST_MSC_DEV not found ..."
		exit 1;
	fi

	if [ ! -d "$TEST_LOG" ]; then
		mkdir -p $TEST_LOG
	fi

	if [ ! -f "$W_DATA" ]; then
		echo "Create random data ..."
		test_opt "random" $DATA_SCR $W_DATA
		cp $W_DATA $TEST_LOG
	fi

	#TEST_FILE_SIZE=`du -b $W_DATA | awk '{print int($1/1024)}'`
	TEST_FILE_SIZE=`du -h $W_DATA | awk '{print $1}'`
	echo "################################"
	echo "Test start: "
	echo "Test file: $TEST_FILE_SIZE"
	echo "Test log: $TEST_LOG"
	echo "################################"
}

choose_dev ()
{
	local dev_main_num;
	local dev_num;
	local num;

	local my_device;

	dev_main_num=`ls $DEVICE? | wc -l`     #/dev/mmcblk?
	num=`expr $RANDOM % $dev_main_num`

	my_device="$DEVICE$num""p"             #/dev/mmcblk1p

	dev_num=`ls $my_device* | wc -l`       #/dev/mmcblk1p*
	num=`expr $RANDOM % $dev_num`

	TEST_MSC_DEV="$my_device$num"          #/dev/mmcblk1p3

	echo "  Test device [$TEST_MSC_DEV]"
}

dd_test ()
{
	local COUNT=1
	local PASS_COUNT=1

	test_pre

	while true; do
		local seek_range;
		seek_range=120
		# read and write card address range 0 ~ seek_range
		# minimun partition 120*512 + size = 60K + size
		MSC_DEV_SEEK=`expr $RANDOM % $seek_range`
		echo -n "========= Test count [$COUNT]"
		echo " seek=$MSC_DEV_SEEK =========="

		if [ $DEBUG -eq 0 ]; then
			choose_dev
		fi

		echo "Write data ..."
		test_opt "write" $W_DATA $TEST_MSC_DEV $MSC_DEV_SEEK
		sync
		echo "Read data ..."
		test_opt "read" $TEST_MSC_DEV $R_DATA $MSC_DEV_SEEK
		echo "Diff file ..."
		file_diff $W_DATA $R_DATA $COUNT $MSC_DEV_SEEK $TEST_MSC_DEV
		if [ $? -eq 0 ]; then
			echo "[PASS]: Test data ok ($PASS_COUNT)"
			PASS_COUNT=`expr $PASS_COUNT + 1`
		fi
		rm $R_DATA
		COUNT=`expr $COUNT + 1`

		trap "test_result  "dd" $COUNT $PASS_COUNT `expr $COUNT - $PASS_COUNT`" INT
	done
}

my_mount ()
{
	if [ $DEBUG -eq 0 ] && [ ! -b $1 ]; then
		echo "Device: $TEST_MSC_DEV not found ..."
		exit 1;
	fi

	if [ ! -d $MOUNT_POINTER ]; then
		mkdir -p $MOUNT_POINTER
	fi

	if [ $DEBUG -eq 0 ]; then
		echo "mount -t $FORMAT $1 $MOUNT_POINTER"
		mount -t $FORMAT $1 $MOUNT_POINTER
		if [ $? -ne 0 ]; then
			echo "mount $1 devive fail ..."
			exit 1;
		fi
	fi
}

my_umount ()
{
	if [ $DEBUG -eq 0 ]; then
		echo "umount $1"
		umount $1
		if [ $? -ne 0 ]; then
			echo "umount $1 device fail ..."
			exit 1;
		fi
	fi
}

copy_to_card ()
{
	if [ -f $MOUNT_POINTER/aaa.img ]; then
		rm $MOUNT_POINTER/aaa.img
	fi

	echo "cp $W_DATA $MOUNT_POINTER"
	cp $W_DATA $MOUNT_POINTER
	if [ $? -ne 0 ]; then
		echo "[ERROR]: copy_to_card file error !!!"
		exit 1;
	fi
}

copy_from_card ()
{
	if [ ! -f $MOUNT_POINTER/aaa.img ]; then
		copy_to_card;
	fi

	echo "cp $MOUNT_POINTER/aaa.img $R_DATA"
	cp $MOUNT_POINTER/aaa.img $R_DATA
	if [ $? -ne 0 ]; then
		echo "[ERROR]: copy_from_card file error !!!"
		exit 1;
	fi
}

copy_file ()
{
	case $1 in
		"to_card" )
			md5sum $W_DATA > $TEST_LOG/md5sum.log
			copy_to_card;
			;;
		"from_card" )
			md5sum $MOUNT_POINTER/aaa.img > $TEST_LOG/md5sum.log
			copy_from_card;
			;;
	esac

	echo "~~~~sync~~~~"
	sync

	case $1 in
		"to_card" )
			md5sum $MOUNT_POINTER/aaa.img >> $TEST_LOG/md5sum.log
			;;
		"from_card" )
			md5sum $R_DATA >> $TEST_LOG/md5sum.log
			;;
	esac

	echo "md5sum -c $TEST_LOG/md5sum.log > $TMP_OUT"
	md5sum -c $TEST_LOG/md5sum.log > $TMP_OUT
	if [ $? -ne 0 ]; then
		echo "[ERROR]: one copy_$1 test data error !!!"
		my_umount $2
		exit 1;
	else
		echo "Data copy_$1 OK ..."
	fi
}

copy_test ()
{
	local COUNT=1
	local COUNT_TO_CARD=1
	local COUNT_FROM_CARD=1

	test_pre

	while true; do
		echo "========= Test count [$COUNT] =========="
		echo "    test copy_to_card count [$COUNT_TO_CARD]"
		echo "    test copy_from_card count [$COUNT_FROM_CARD]"

		echo "mount $TEST_MSC_DEV device ..."
		my_mount $TEST_MSC_DEV

		copy_file "to_card" $TEST_MSC_DEV
		if [ $? -ne 0 ]; then
			echo "[ERROR]: Data copy_to_card faill !!!"
		else
			COUNT_TO_CARD=`expr $COUNT_TO_CARD + 1`
		fi

		echo ""
		rm $TEST_LOG/md5sum.log

		copy_file "from_card" $TEST_MSC_DEV
		if [ $? -ne 0 ]; then
			echo "[ERROR]: Data copy_from_card fail !!!"
		else
			COUNT_FROM_CARD=`expr $COUNT_FROM_CARD + 1`
		fi

		echo "umount $TEST_MSC_DEV device ..."
		my_umount $TEST_MSC_DEV
		if [ $? -eq 0 ]; then
			COUNT=`expr $COUNT + 1`
		fi

		trap "test_result "copy" $COUNT "null"  "null" " INT
	done
}

clear_test_file ()
{
	echo "Delete old test file ..."

	case $1 in
		"-cp" )
			umount $TEST_MSC_DEV
			;;
	esac

	echo "rm -rf  $W_DATA $R_DATA $TEST_LOG $MOUNT_POINTER"
	rm -rf  $W_DATA $R_DATA $TEST_LOG $MOUNT_POINTER

	exit 1;
}

set_data_size ()
{
	DATA_SIZE=$1

	if [ -f "$W_DATA" ]; then
		rm $W_DATA
	fi
}

usage ()
{
	echo "Usage: $0"
	echo "  Arg:"
	echo "    -h help"
	echo "    -d debug"
	echo "    -c clear old test file"
	echo "    -s random data size(unit MB). Default size 1MB"
	echo "  Test Case:"
	echo "    -dd   dd test case"
	echo "    -cp   cp test case, Default mount format ext4"
	echo ""
	echo "Sample:"
	echo " $0 -s 20 -dd   # dd file size 20MB"
	echo " $0 -d -dd      # debug"
	echo " $0 -dd  # dd test"
	echo " $0 -cp  # cp test"
	echo ""
	echo "Note: minimun partition size is 3M, so the multi partition random "
	echo "      test data should not be greater than 3084288/1024/1024=2.94M"
	echo "      size: 3 * 1024 * 1024 - 120 * 512 = 3084288 Byte"

	exit 1;
}

check_arg ()
{
	if [ $1 -ne $2 ]; then
		usage;
	fi

	case $3 in
		"-dd" )
			dd_test
			;;
		"-cp" )
			copy_test
			;;
	esac
}

#main
if [ $# -eq 0 ]; then
	usage;
fi

case $1 in
	"-h" )
		usage
		;;
	"-d" )
		debug_test
		check_arg $# 2 $2
		;;
	"-c" )
		clear_test_file $2
		;;
	"-s" )
		set_data_size $2
		check_arg $# 3 $3
		;;
	"-dd" )
		dd_test
		;;
	"-cp" )
		copy_test
		;;
esac


