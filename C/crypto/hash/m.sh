#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2021年09月16日 星期四 19时28分15秒
# Description	:
##########################################################

set -x

gcc ./sha512_test.c -o ./sha512_test.out  `pkg-config --libs openssl`
