#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年11月02日 星期一 11时11分28秒
# Description	:
##########################################################


rpcgen   trans.x

gcc -Wall -o trans_server.out server.c trans_svc.c trans_sif.c
gcc -Wall -o trans_client.out client.c trans_clnt.c trans_cif.c
