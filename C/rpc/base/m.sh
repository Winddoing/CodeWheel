#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年09月29日 星期日 22时16分44秒
# Description	:
##########################################################

# sudo apt install portmap
# sudo systemctl status portmap.service

if [ x$1 == "x"x ]; then
    rpcgen rpc_test.x
    rpcgen -Sc -o client.c rpc_test.x
    rpcgen -Ss -o server.c rpc_test.x
fi

gcc -o client.out rpc_test_clnt.c rpc_test_xdr.c client.c
gcc -o server.out rpc_test_svc.c rpc_test_xdr.c server.c 
