#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : ymodem_xfer_test.sh
#  Author       : wqshao
#  Created Time : 2025-01-06 10:10:57
#  Description  :
##########################################################

# 虚拟串口路径
PTY1="/dev/vpty1"
PTY2="/dev/vpty2"

# 创建虚拟串口对
sudo socat -d -d pty,raw,echo=0,link=${PTY1} pty,raw,echo=0,link=${PTY2} &
SOCAT_PID=$!

# 等待虚拟串口创建完成
echo "等待虚拟串口初始化..."
sleep 2

# 检查虚拟串口是否创建成功
if [[ ! -e ${PTY1} || ! -e ${PTY2} ]]; then
    echo "虚拟串口创建失败！"
    kill ${SOCAT_PID}
    exit 1
fi

sudo stty -F $PTY1 115200 cs8 -parenb -cstopb -crtscts
sudo stty -F $PTY2 115200 cs8 -parenb -cstopb -crtscts

# 创建测试文件
TESTFILE=/tmp/testfile.txt
rm -f ${TESTFILE}
echo "uart ymodem xfer test, 123" > ${TESTFILE}

# 发送文件（YMODEM） -v 调试
echo "启动 sz 发送文件..."
sudo bash -c "sz --ymodem ${TESTFILE} > ${PTY1} < ${PTY1}" &
SZ_PID=$!


# 接收文件（YMODEM）
echo "启动 rz 接收文件..."
RECEIVED_FILE=testfile.txt
rm -f ${RECEIVED_FILE}
sudo bash -c "rz < ${PTY2} > ${PTY2}" &
RZ_PID=$!


echo "PID, sz=$SZ_PID, rz=$RZ_PID"

# 等待完成
wait ${SZ_PID}
wait ${RZ_PID}

# 检查文件是否传输成功
echo "检查文件内容..."
cat ${TESTFILE}
cat ${RECEIVED_FILE}

if cmp -s ${TESTFILE} ${RECEIVED_FILE}; then
    echo "文件传输成功！"
else
    echo "文件传输失败！"
fi

# 清理
sudo kill ${SOCAT_PID}
rm -f ${PTY1} ${PTY2} ${TESTFILE} ${RECEIVED_FILE}
