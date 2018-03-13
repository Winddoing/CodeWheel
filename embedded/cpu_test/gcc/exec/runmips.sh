#!/bin/sh

SIM=

EXEC=../bin/gcc

#let count=100
#while [ $count -ne 0 ];do
$SIM $EXEC -g -O1 ../data/1.c >& /dev/null
#let count=count-1
#done


#  /* #ifdef XXX_HARD */
#  /*   int argc = 5; */
#  /*   char *argv[] = {"sgcc.exe", "-dcfgj", "-g", "-O4", "../1.c"}; */
#  /* #else // NORMAL or EASY */
#  /*   int argc = 3; */
#  /*   char *argv[]={"sgcc.exe", "-O4", "../1.c"}; */
#  /* #endif */
