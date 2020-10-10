#!/bin/sh

A=abc
B="$A"
C='$A'

echo $A
echo $B
echo $C
unset A B C  #
