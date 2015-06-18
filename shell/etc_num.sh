#!/bin/sh

D_NUM=0
L_NUM=0
F_NUM=0

for FILE in  /etc/*
do
	if [ -d $FILE ]
	then 
		D_NUM=`expr $D_NUM + 1`
	elif [ -L $FILE ]
	then
		L_NUM=`expr $L_NUM + 1`
	elif [ -f $FILE ]
	then
		F_NUM=`expr $F_NUM + 1`
	fi
done

echo "D_num = $D_NUM, L_num = $L_NUM, F_num = $F_NUM, num = `expr $D_NUM + $L_NUM + $F_NUM`"

unset D_NUM F_NUM L_NUM  


