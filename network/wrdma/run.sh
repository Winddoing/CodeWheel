#!/bin/bash

if [ x"$1" == x"s" ]; then
	./tst.out -d mlx5_0 -g 0
elif [ x"$1" == x"c" ]; then 
	./tst.out -d mlx5_0 -g 0 192.168.2.4
else
	echo "$0 [s/c]"
	echo "  s: server"
	echo "  c: client"
fi
