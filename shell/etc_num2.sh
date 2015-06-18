#!/bin/sh

echo -n "Input a directory:"
read DIR

if [ ! -d $DIR ]
then
	echo \"$DIR\" is not a directory
	unset DIR
	exit 1
fi

D_NUM=0
L_NUM=0
F_NUM=0
E_NUM=0

for FILE in  "$DIR"/*
do
	if [ -d "$FILE" ]
	then 
		D_NUM=`expr $D_NUM + 1`
	elif [ -L "$FILE" ]
	then
		L_NUM=`expr $L_NUM + 1`
	else
		F_NUM=`expr $F_NUM + 1`
		if [ -x "$FILE" ]
		then
			E_NUM=`expr $E_NUM + 1`
		fi
	fi
done

echo "D_num = $D_NUM, L_num = $L_NUM, F_num = $F_NUM, E_num = $E_NUM, num = `expr $D_NUM + $L_NUM + $F_NUM`"

unset D_NUM F_NUM L_NUM  DIR

