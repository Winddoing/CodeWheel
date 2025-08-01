#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : load_bootrom.sh
#  Author       : wqshao
#  Created Time : 2025-07-08 09:22:03
#  Description  :
##########################################################

whereis JLinkExe > /dev/null
if [ $? -ne 0 ]; then
	echo "JLinkExe command does not exist."
	exit -1
fi

JLINK_CMD_FILE="/tmp/bootrom.jlink"

cat > $JLINK_CMD_FILE << EOF
ExitOnError 1          // 错误时自动退出
Device CORTEX-A53
Speed 4000
JTAGConf -1,-1       	// JTAG自动配置
SelectInterface JTAG    // 选择JTAG
Connect
LoadFile boot.bin 0xFFFF0000
SetPC 0xFFFF0000
Go
Sleep 2000
QC
Exit
EOF

JLinkExe -AutoConnect 1 -NoGui 1 -CommandFile $JLINK_CMD_FILE

rm $JLINK_CMD_FILE
