#/bin/sh

SUM=1
NUM=1

while [ "$NUM" -lt 100 ]
do
	NUM=`expr $NUM + 1`
	SUM=`expr $SUM + $NUM`
done

echo "1+2+....+100 = $SUM"

unset SUM NUM
	
