#!/bin/sh
# genext2fs wrapper calculating needed blocks/inodes values if not specified

GENEXT2FS=genext2fs

usage()
{
	echo "usage: $0 -b 0 -N 0 -d rootfs"
	echo "	-N inodes (leave at 0 for auto calculation)"
	echo "	-b size in blocks (leave at 0 for auto calculation)"
	echo "		Specify file system size(unit MB)"
	echo "	-d target dir"

	echo "100M space filesystem:"
	echo "	$0 -b 100 -N 498 -d rootfs"

	exit 1
}
while getopts x:d:D:b:i:N:m:g:e:zfqUPhVv f
do
    case $f in
	b)
		blocks=$OPTARG ;;
	N)
		inodes=$OPTARG ;;
	d)
		TARGET_DIR=$OPTARG ;;
	h)
		usage ;;
	?)
		usage
		exit 1 ;;
    esac
done

BLOCKS=`expr $blocks \* 1024`
INODES=$inodes
OUTPUT=$TARGET_DIR/../system.ext4

# calculate needed inodes
if [ $INODES -eq 0 ];
then
   INODES=$(find $TARGET_DIR | wc -l)
   INODES=$(expr $INODES + 400)
   # set -- $@ -N $INODES
   echo "Computing file system inodes :$INODES"
fi

# calculate needed blocks
if [ $BLOCKS -eq 0 ];
then
    # size ~= superblock, block+inode bitmaps, inodes (8 per block), blocks
    # we scale inodes / blocks with 10% to compensate for bitmaps size + slack
    BLOCKS=$(du -s -c -k $TARGET_DIR | grep total | sed -e "s/total//")
    BLOCKS=$(expr 500 + \( $BLOCKS + $INODES / 8 \) \* 11 / 10)
    # we add 1300 blocks (a bit more than 1 MiB, assuming 1KiB blocks) for
    # the journal if ext3/4
    # Note: I came to 1300 blocks after trial-and-error checks. YMMV.
	#if [ ${GEN} -ge 3 ]; then
    #   BLOCKS=$(expr 1300 + $BLOCKS )
    #fi
    # set -- $@ -b $BLOCKS
	echo "Computing file system space size :$BLOCKS KB"
fi

echo "size: $BLOCKS KB"
echo "inodes: $INODES"
echo "cmd: "$GENEXT2FS" -d "$TARGET_DIR" -b "$BLOCKS" -N "$INODES" "$OUTPUT
echo "make ...."

$GENEXT2FS -d $TARGET_DIR -b $BLOCKS -N $INODES $OUTPUT

echo "File system make complete space $BLOCKS KB"

echo "Output ext4 filesystem: $OUTPUT"
