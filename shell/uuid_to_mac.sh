#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : uuid_to_mac.sh
#  Author       : wqshao
#  Created Time : 2025-07-31 19:54:23
#  Description  :
##########################################################

# 生成UUID并去除连字符
UUID=$(uuidgen | tr -d '-')

# 截取前12个字符作为MAC基础
MAC_RAW=${UUID:0:12}
echo "MAC_RAW: $MAC_RAW"

# 截取前2个字符
MAC_1BYTE_HEX=${MAC_RAW:0:2}
echo "MAC_1BYTE_HEX: $MAC_1BYTE_HEX"

# 转十进制
MAC_1BYTE_DEC=$((16#$MAC_1BYTE_HEX))

# 设置本地管理位
# 为避免与官方分配的 MAC 冲突，需将首字节的 第 2 个最低有效位 置为 1（表示本地管理地址）：
# bit[0]: 0表示单播，1表示多播
# bit[1]: 1表示本地管理的MAC地址,0表示全局唯一地址
#MAC_1BYTE_DEC=$((MAC_1BYTE_DEC & 0xFE | 0x02))  # 确保最低位为0（单播），次低位为1（本地）
MAC_1BYTE_DEC=$(((16#$MAC_1BYTE_HEX) & 0xFE | 0x02))  # 确保最低位为0（单播），次低位为1（本地）

# 转为十六进制，全部大写
MAC_1BYTE_HEX=$(printf "%02X" $MAC_1BYTE_DEC)
echo "MAC_1BYTE_HEX: $MAC_1BYTE_HEX"


MAC_2BYTE_HEX=$(printf "%02X" \'V)
echo "MAC_2BYTE_HEX=$MAC_2BYTE_HEX"

MAC_3BYTE_HEX=$(printf "%02X" \'X)
echo "MAC_3BYTE_HEX=$MAC_3BYTE_HEX"

# 组合为标准MAC格式
#MAC_ADDR="${MAC_1BYTE_HEX}:${MAC_RAW:2:2}:${MAC_RAW:4:2}:${MAC_RAW:6:2}:${MAC_RAW:8:2}:${MAC_RAW:10:2}"
MAC_ADDR="${MAC_1BYTE_HEX}:${MAC_2BYTE_HEX}:${MAC_3BYTE_HEX}:${MAC_RAW:6:2}:${MAC_RAW:8:2}:${MAC_RAW:10:2}"

echo "mac address: $MAC_ADDR"



# 定义 device_id 获取函数
get_device_id() {
	## CPU序列号
	#device_id=$(cat /proc/cpuinfo | grep Serial | awk '{print $3}')
	## 机器的唯一标识
	#device_id=$(cat /etc/machine-id)
	## eMMC/SD 卡 CID (适用于树莓派)
	#device_id=$(cat /sys/block/mmcblk0/device/cid)
	## NVMe SSD
	#device_id=$(nvme id-ctrl /dev/nvme0 | grep "SN" | awk '{print $3}')
	#device_id=$(cat /sys/block/nvme0n1/device/serial)
	#ls /sys/block/nvme*/device/serial | xargs -I{} sh -c 'echo -n "{}: "; cat {}'

  	# 尝试多种硬件来源
  	if [ -f /proc/cpuinfo ]; then
  	        id=$(grep -m1 "Serial" /proc/cpuinfo | awk '{print $3}')
  	        [ -n "$id" ] && echo "$id" && return
  	fi

  	if [ -f /sys/block/mmcblk0/device/cid ]; then
  	        id=$(cat /sys/block/mmcblk0/device/cid)
  	        [ -n "$id" ] && echo "$id" && return
  	fi

  	if [ -f /sys/block/nvme0n1/device/serial ]; then
  	        id=$(cat /sys/block/nvme0n1/device/serial)
  	        [ -n "$id" ] && echo "$id" && return
  	fi

  	## 最终回退：系统持久化ID
  	#if [ ! -f /etc/device_id ]; then
  	#  head -c 256 /dev/urandom | sha256sum | cut -d' ' -f1 > /etc/device_id
  	#fi
  	#cat /etc/device_id
}

# 获取 device_id
device_id=$(get_device_id)

# 生成MAC地址 (优化版)
MAC=$(
	# 第一部分：设置本地管理和单播
	first=$(((16#$(uuidgen | tr -d '-' | cut -c 1-2)) & 0xFE | 0x02))
	first=$(printf "%02X" $first)

	# 第二部分：UUID片段
	prefix="$(uuidgen | tr -d '-' | cut -c 3-4):$(uuidgen | tr -d '-' | cut -c 5-6)"

	# 第三部分：基于device_id的哈希
	suffix=$(echo -n "$device_id" | md5sum | cut -c 1-6 | sed 's/../&:/g; s/:$//')
	
	echo "${first}:${prefix}:${suffix}"
)

echo "MAC: $MAC"
