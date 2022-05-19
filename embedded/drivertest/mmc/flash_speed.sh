#!/bin/sh

usage()
{
	echo "Test the read and write speed of flash."
	echo ""
	echo "Usage:"
	echo "	flash_speed.sh /dev/mtd0		#test spiflash"
	echo "	flash_speed.sh /dev/mmcblk0p4		#test sd/emmc"
	echo "	flash_speed.sh /dev/sda1		#test udisk"
	echo ""

	exit 0
}

if [ $# -ne 1 ]; then
	usage
fi

TST_DEV=$1
TST_TYPE="x"

echo $TST_DEV | grep "mtd" > /dev/null && TST_TYPE="spi"
echo $TST_DEV | grep "mmc" > /dev/null && TST_TYPE="mmc"
echo $TST_DEV | grep "sd"  > /dev/null && TST_TYPE="sda"
[ $TST_TYPE = "x" ] && usage
echo "Current test [$TST_TYPE]"


TST_DATA_BS=$((1*1024*1024))  #1MB

TST_SKIP=$((1*1024*1024)) #1MB
ERASE_BLK_SZ=$((64*1024)) #64KB


# read test
for sz_mb in 15 20 25
do
	[ $TST_TYPE != "spi" ] && sz_mb=$((sz_mb * 10))
	sz=$((sz_mb * 1024 * 1024))
	#echo "sz_mb=$sz_mb, sz=$sz"
	tst_data_cnt=$((sz / TST_DATA_BS))

	printf "Test read data : %3.3s MB, " $sz_mb

	start_time=$(date +"%s")
	dd if=$TST_DEV of=/dev/null bs=$TST_DATA_BS count=$tst_data_cnt 2> /dev/null
	end_time=$(date +"%s")
	tdiff=$((end_time - start_time))

	#echo "tdiff=$tdiff, size=$((TST_DATA_BS * tst_data_cnt))"
	speed=$(echo "scale=4;$sz_mb/$tdiff"| bc)
	printf "time: %2d s, speed: %5.2f MB/s\n" $tdiff $speed
done


flasherase()
{
	local erase_blk_size=$1

	flash_erase $TST_DEV $TST_SKIP $erase_blk_size 1>&2 > /dev/null
}

# write test
for sz_mb in 5 10 15
do
	[ $TST_TYPE != "spi" ] && sz_mb=$((sz_mb * 10))
	sz=$((sz_mb * 1024 * 1024))
	#echo "sz_mb=$sz_mb, sz=$sz"
	tst_data_cnt=$((sz / TST_DATA_BS))
	seek_cnt=$((TST_SKIP / TST_DATA_BS))
	erase_blk_sz=$((sz / ERASE_BLK_SZ + 1))
	[ $TST_TYPE = "spi" ] && flasherase $erase_blk_sz

	printf "Test wirte data: %3.3s MB, " $sz_mb

	start_time=$(date +"%s")
	dd if=/dev/zero of=$TST_DEV bs=$TST_DATA_BS count=$tst_data_cnt seek=$seek_cnt 2> /dev/null
	end_time=$(date +"%s")
	tdiff=$((end_time - start_time))

	#echo "tdiff=$tdiff, size=$((TST_DATA_BS * tst_data_cnt))"
	speed=$(echo "scale=4;$sz_mb/$tdiff"| bc)
	printf "time: %2d s, speed: %5.2f MB/s\n" $tdiff $speed
done


echo "over"
