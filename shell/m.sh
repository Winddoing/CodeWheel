#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : m.sh
#  Author       : wqshao
#  Created Time : 2022-07-29 15:18:44
#  Description  :
#  ./m.sh -c; ./m.sh -db; ./m.sh -d -b
##########################################################

set -- $(getopt -q dbc "$@")
while [ -n "$1" ]
do
        case "$1" in
            -d) echo "Deploy ..."

                shift ;;
            -b) echo "Build  ..."

                shift ;;
            -c) echo "Clean  ..."

                shift ;;
            --) shift
                break ;;
            -*) echo "Nothing to do";;
        esac
done

