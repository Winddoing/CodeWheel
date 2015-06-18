#!/bin/sh

echo -n "Input a number : "

read NUM

#if [ "$NUM" -ge 80 -a "$NUM" -le 100 ]
#then
#	echo A
#elif [ "$NUM" -ge 60 -a "$NUM" -le 79 ]
#then 
#	echo B
#elif [ "$NUM" -ge 0  -a "$NUM" -le 59 ]
#then 
#	echo C
#else
#	echo error
#fi

#unset NUM


if [ "$NUM" -gt 100 -o "$NUM" -lt 0 ]
then 
	GRADE="Invalid socre"
elif [ "$NUM" -ge 80 ]
then
	GRADE="A"
elif [ "$NUM" -ge 60 ]
then
	GRADE="B"
else
	GRADE="C"
fi

echo The score is $NUM, the grade is $GRADE

unset NUM GRADE

