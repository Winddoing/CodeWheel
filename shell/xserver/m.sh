#!/bin/bash
##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2020年12月26日 星期六 14时53分21秒
# Description	:
##########################################################


sudo get-edid > edid.bin

cat build_x_server.sh edid.bin > deploy_x_server.sh

shc -v -f deploy_x_server.sh -o deploy_x_server

rm edid.bin deploy_x_server.sh -rf
