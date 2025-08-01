#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : pcie_net_list.sh
#  Author       : wqshao
#  Created Time : 2025-07-21 14:01:55
#  Description  :
##########################################################

# 定义PCI设备根目录
PCI_ROOT="/sys/devices/pci0000:00"
#PCI_ROOT="/sys/devices/pci0000:3a"

# 打印表头
printf "%-15s %-15s %-15s %s\n" "Bus ID" "Device Name" "Interface" "MAC Address"
printf "==================================================================\n"

# 查找所有包含net目录的PCI设备路径
find "$PCI_ROOT" -type d -name 'net' 2>/dev/null | while read -r net_dir; do
# 获取设备路径（net目录的父目录）
device_dir=$(dirname "$net_dir")

    # 提取实际的PCI设备地址（处理嵌套路径）
    pci_address=$(echo "$device_dir" | grep -oE '0000:[[:xdigit:]]{2}:[[:xdigit:]]{2}\.[[:xdigit:]]' | tail -1)

    # 如果无法提取有效地址，尝试其他格式
    if [ -z "$pci_address" ]; then
	    # 尝试匹配不带域的格式 (XX:XX.X)
	    pci_address=$(echo "$device_dir" | grep -oE '[[:xdigit:]]{2}:[[:xdigit:]]{2}\.[[:xdigit:]]' | tail -1)
    fi

    # 如果仍然无法提取，使用basename作为后备
    if [ -z "$pci_address" ]; then
	    pci_address=$(basename "$device_dir")
    fi

    # 遍历该设备下的所有网络接口
    for interface_dir in "$net_dir"/*/; do
	    # 获取接口名
	    interface_name=$(basename "$interface_dir")

	# 检查MAC地址文件是否存在
	mac_file="${interface_dir}address"
	if [[ -f "$mac_file" ]]; then
		# 读取MAC地址
		mac_address=$(cat "$mac_file" 2>/dev/null | tr '[:lower:]' '[:upper:]' | tr -d '[:space:]')

	    # 验证MAC地址格式
	    if [[ "$mac_address" =~ ^([0-9A-F]{2}:){5}[0-9A-F]{2}$ ]]; then
		    # 以列表格式输出
		    printf "%-15s %-15s %-15s %s\n" \
			    "$pci_address" \
			    "$(basename "$device_dir")" \
			    "$interface_name" \
			    "$mac_address"
	    fi
	fi
done
done

