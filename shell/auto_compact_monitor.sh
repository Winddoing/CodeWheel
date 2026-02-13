#!/bin/bash
##########################################################
# Copyright (C) 2026 wqshao All rights reserved.
#  File Name    : auto_compact_monitor.sh
#  Author       : wqshao
#  Created Time : 2026-02-06 14:07:42
#  Description  :
##########################################################

# 嵌入式Linux内存碎片+IO监控自动规整脚本
# 适配：无swap、内存碎片化严重、eMMC/SD卡存储的嵌入式系统

########################### 可配置参数（根据你的系统调整） ###########################
# 1. 高阶页监控：Normal Zone，阶数9对应512页=2MB连续内存，阶数10对应1024页=4MB（二选一）
HIGH_ORDER="9"          # 监控阶数（9=2MB，10=4MB，根据你的需求调整）
HIGH_ORDER_THRESHOLD=5  # 高阶页低于此值，触发规整
# 2. 主缺页监控：pgmajfault突增阈值（单位：次/监控周期），反映磁盘IO根源
MAJ_FAULT_INC_THRESHOLD=500
# 3. 磁盘IO监控：读IO阈值（单位：MB/s），超过则触发规整
DISK_READ_IO_THRESHOLD=2
# 4. 磁盘设备名（嵌入式常用：mmcblk0=eMMC，mmcblk1=SD卡，sda=U盘）
DISK_DEV="mmcblk0"
# 5. 监控间隔（秒）：嵌入式建议30~60秒，避免占用CPU
MONITOR_INTERVAL=30
# 6. 规整冷却时间（秒）：规整后N秒内不再次触发，避免频繁执行
COMPACT_COOLDOWN=600
# 7. 日志路径（嵌入式建议放tmpfs，避免写磁盘）
LOG_FILE="/tmp/memory_compact.log"
###################################################################################

# 初始化变量
LAST_MAJ_FAULT=0
LAST_COMPACT_TIME=0
# 初始化日志头
echo "===== 内存碎片监控自动规整脚本启动 $(date) =====" >> $LOG_FILE
echo "配置：高阶页阶数=$HIGH_ORDER，阈值=$HIGH_ORDER_THRESHOLD，监控间隔=$MONITOR_INTERVAL秒" >> $LOG_FILE
echo "=====================================================================" >> $LOG_FILE

# 函数1：获取Normal Zone指定阶数的连续页数量
get_high_order_count() {
	cat /proc/buddyinfo | grep Normal | awk -v order="$HIGH_ORDER" '{print $(10+order)}'
}

# 函数2：获取主缺页中断总数（/proc/vmstat）
get_maj_fault() {
	cat /proc/vmstat | grep pgmajfault | awk '{print $2}'
}

# 函数3：获取磁盘读IO（MB/s），兼容无iostat的系统
get_disk_read_io() {
	if command -v iostat >/dev/null 2>&1; then
		iostat -d -k 1 2 | grep "$DISK_DEV" | grep -v "boot" | tail -n1 | awk '{print $3/1024}' # 转换为MB/s
	else
		echo "0" # 无iostat则返回0，不触发IO阈值
	fi
}

# 函数4：获取CPU iowait占用率（%）
get_iowait() {
	top -bn1 | grep "^CPU:" | awk '{for(i=1;i<=NF;i++) if($i=="io") print $(i-1)}' | tr -d '%'
}

# 主监控循环
while true; 
do
	CURRENT_TIME=$(date +%s)
	# 采集当前指标
	CURRENT_HIGH_ORDER=$(get_high_order_count)
	CURRENT_MAJ_FAULT=$(get_maj_fault)
	CURRENT_DISK_IO=$(get_disk_read_io)
	CURRENT_IOWAIT=$(get_iowait)
	# 计算主缺页增量
	MAJ_FAULT_INC=$((CURRENT_MAJ_FAULT - LAST_MAJ_FAULT))
	LAST_MAJ_FAULT=$CURRENT_MAJ_FAULT

	# 打印实时指标（控制台输出，可选）
	echo "【实时监控】$(date) | 高阶${HIGH_ORDER}页：$CURRENT_HIGH_ORDER | 主缺页增量：$MAJ_FAULT_INC | 磁盘读IO：${CURRENT_DISK_IO}MB/s | iowait：${CURRENT_IOWAIT}%"
	
	# 触发规整条件：满足任一条件 + 冷却时间已过
	TRIGGER=0
	TRIGGER_REASON=""
	# 条件1：高阶页低于阈值
	if [ $CURRENT_HIGH_ORDER -lt $HIGH_ORDER_THRESHOLD ]; then
	        TRIGGER=1
	        TRIGGER_REASON="高阶${HIGH_ORDER}页不足（$CURRENT_HIGH_ORDER < $HIGH_ORDER_THRESHOLD）"
	fi
	# 条件2：主缺页突增（反映页缓存失效，磁盘IO根源）
	if [ $MAJ_FAULT_INC -gt $MAJ_FAULT_INC_THRESHOLD ]; then
	        TRIGGER=1
	        TRIGGER_REASON="主缺页突增（$MAJ_FAULT_INC > $MAJ_FAULT_INC_THRESHOLD）"
	fi
	# 条件3：磁盘读IO超过阈值
	if [ $(echo "$CURRENT_DISK_IO > $DISK_READ_IO_THRESHOLD" | bc) -eq 1 ]; then
	        TRIGGER=1
	        TRIGGER_REASON="磁盘读IO过高（${CURRENT_DISK_IO}MB/s > ${DISK_READ_IO_THRESHOLD}MB/s）"
	fi
	# 检查冷却时间
	if [ $((CURRENT_TIME - LAST_COMPACT_TIME)) -lt $COMPACT_COOLDOWN ]; then
	        TRIGGER=0
	fi
	
	# 执行规整并记录日志
	if [ $TRIGGER -eq 1 ]; then
	        echo "===== 触发内存规整：$TRIGGER_REASON $(date) =====" >> $LOG_FILE
	        echo "规整前指标：高阶${HIGH_ORDER}页=$CURRENT_HIGH_ORDER，主缺页增量=$MAJ_FAULT_INC，磁盘IO=${CURRENT_DISK_IO}MB/s，iowait=${CURRENT_IOWAIT}%" >> $LOG_FILE
	
	    # 执行内存规整
	    echo 1 > /proc/sys/vm/compact_memory
	    LAST_COMPACT_TIME=$CURRENT_TIME
	
	    # 采集规整后指标
	    sleep 2 # 等待规整完成
	    AFTER_HIGH_ORDER=$(get_high_order_count)
	    AFTER_DISK_IO=$(get_disk_read_io)
	    AFTER_IOWAIT=$(get_iowait)
	
	    echo "规整后指标：高阶${HIGH_ORDER}页=$AFTER_HIGH_ORDER，磁盘IO=${AFTER_DISK_IO}MB/s，iowait=${AFTER_IOWAIT}%" >> $LOG_FILE
	    echo "=====================================================================" >> $LOG_FILE
	    echo "【自动规整完成】高阶页从$CURRENT_HIGH_ORDER → $AFTER_HIGH_ORDER，磁盘IO从${CURRENT_DISK_IO} → ${AFTER_DISK_IO}MB/s"
	fi
	
	# 等待下一个监控周期
	sleep $MONITOR_INTERVAL
done
