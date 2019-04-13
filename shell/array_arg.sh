#!/bin/bash
##########################################################
# File Name		: a.sh
# Author		: winddoing
# Created Time	: 2019年04月13日 星期六 15时37分47秒
# Description	:
##########################################################

#! /bin/bash
function f() {
    name=$1[@]
    a=("${!name}")

    for i in "${a[@]}" ; do
        echo "$i"
    done
}

x=("one two" "LAST")

f x 
