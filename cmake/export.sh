#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : export.sh
#  Author       : wqshao
#  Created Time : 2023-01-28 16:59:16
#  Description  :
##########################################################

LIB_NAME=$1
LIB_NAME=${LIB_NAME:-"xxxx"}

echo "LIB_NAME: $LIB_NAME"

if [ -d $LIB_NAME ]; then
	echo "$LIB_NAME directory already exists."
	exit
fi

cp -arp ./temp-lib $LIB_NAME

cd $LIB_NAME

find -name "CMakeLists.txt" | xargs sed -i "s/TEMP/$LIB_NAME/g"

for file in `find . -name "*TEMP*"`
do
	echo "--$file"
	echo "$file" | grep "package"
	if [ $? -eq 0 ]; then
		continue
	fi
	echo "Edit ..."
	rm $file
	new_file=`echo "$file" | sed "s/TEMP/$LIB_NAME/g"`
	vim $new_file 
done

cd -


