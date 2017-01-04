#!/bin/sh
PWD=`pwd`

DATA_SCR=/dev/urandom
W_DATA=/dev/aaa.img
R_DATA=/dev/bbb.img
TEST_MSC_DEV=/dev/mmcblk0p7
#W_DATA=aaa.img
#R_DATA=bbb.img
#TEST_MSC_DEV=aaaaa

MSC_DEV_SEEK=0
DATA_SIZE=1 #1M
DD_BS=1024 #KB

TEST_LOG=$PWD/log

test_opt ()
{
	local cnt
	local tmp

	tmp=`expr $DATA_SIZE \* 1024 \* 1024`
	cnt=`expr $tmp / $DD_BS`

	case $1 in
		"random" )
			echo "dd if=$2 of=$3 bs=$DD_BS count=$cnt > /dev/null 2>&1"
			dd if=$2 of=$3 bs=$DD_BS count=$cnt > /dev/null 2>&1
			;;
		"write" )
			echo "dd if=$2 of=$3 seek=$4 bs=$DD_BS count=$cnt > /dev/null 2>&1"
			dd if=$2 of=$3 seek=$4 bs=$DD_BS count=$cnt > /dev/null 2>&1
			;;
		"read" )
			echo "dd if=$2 of=$3 skip=$4 bs=$DD_BS count=$cnt > /dev/null 2>&1"
			dd if=$2 of=$3 skip=$4 bs=$DD_BS count=$cnt > /dev/null 2>&1
			;;
	esac
}

file_diff ()
{
	local file_name

	file_name="error$3_data_opt-$4.img"

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
	if [ ! -b "$TEST_MSC_DEV" ]; then
		echo "file: $TEST_MSC_DEV not found ..."
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
}

dd_test ()
{
	local COUNT=1
	local PASS_COUNT=1

	test_pre

	echo "################################"
	echo "Test start: "
	echo "Test file: `ls -lh "$W_DATA"`"
	echo "Test log: $TEST_LOG"
	echo "################################"

	while true; do
		echo -n "========= Test count [$COUNT]"
		MSC_DEV_SEEK=`expr $RANDOM % 120`
		echo " seek=$MSC_DEV_SEEK =========="
		echo "Write data ..."
		test_opt "write" $W_DATA $TEST_MSC_DEV $MSC_DEV_SEEK
		echo "Read data ..."
		test_opt "read" $TEST_MSC_DEV $R_DATA $MSC_DEV_SEEK
		echo "Diff file ..."
		file_diff $W_DATA $R_DATA $COUNT $MSC_DEV_SEEK
		if [ $? -eq 0 ]; then
			echo "[PASS]: Test data ok ($PASS_COUNT)"
			PASS_COUNT=`expr $PASS_COUNT + 1`
		fi
		rm $R_DATA
		COUNT=`expr $COUNT + 1`
	done
}

clear_test_file ()
{
	echo "Delete old test file ..."
	echo "rm -rf  $W_DATA $R_DATA $TEST_LOG"
	rm -rf  $W_DATA $R_DATA $TEST_LOG

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
	echo "  -h help"
	echo "  -c clear old test file"
	echo "  -s random data size"

	exit 1;
}

#main
case $1 in
	"-h" )
		usage
		;;
	"-c" )
		clear_test_file
		;;
	"-s" )
		set_data_size $2
		;;
esac

dd_test


