#!/bin/bash
##########################################################
# Copyright (C) 2026 wqshao All rights reserved.
#  File Name    : process_monitor1.sh
#  Author       : wqshao
#  Created Time : 2026-03-26 15:27:00
#  Description  :
##########################################################
# monitor_scripts.sh - Monitor scripts in a directory, auto-restart on exit

# ---------- 配置区域 ----------
SCRIPT_DIR="/path/to/scripts"   # 脚本存放目录（请修改为实际路径）
PATTERN="*.sh"                  # 匹配模式（如 "*.sh" 或 "run_*"）
LOG_TAG="monitor_scripts"       # syslog 标签
# -----------------------------

# 全局数组，存储每个脚本的 PID
declare -a pids
# 存储脚本文件路径的数组
declare -a scripts

# 日志函数：同时输出到终端和 syslog
log_message() {
    local msg="$1"
    local timestamp=$(date '+%H:%M:%S')
    echo "[$timestamp] $msg"
    logger -t "$LOG_TAG" "$msg"
}

# 启动一个脚本并记录 PID
# 参数: $1 = 脚本路径, $2 = 索引
start_script() {
    local script_path="$1"
    local idx="$2"
    if [ ! -x "$script_path" ]; then
        log_message "WARNING: $script_path is not executable, trying with bash..."
        bash "$script_path" &
    else
        "$script_path" &
    fi
    local pid=$!
    if kill -0 "$pid" 2>/dev/null; then
        pids[$idx]=$pid
        log_message "Started script $idx: $script_path (PID $pid)"
    else
        log_message "ERROR: Failed to start script $idx: $script_path"
        pids[$idx]=""
    fi
}

# 清理：杀死所有被监控的子进程
cleanup() {
    log_message "Received exit signal, cleaning up all child processes..."
    for pid in "${pids[@]}"; do
        if [ -n "$pid" ] && kill -0 "$pid" 2>/dev/null; then
            kill "$pid" 2>/dev/null
            wait "$pid" 2>/dev/null
        fi
    done
    exit 0
}

# 捕获退出信号
trap cleanup SIGINT SIGTERM EXIT

# ---------- 主逻辑 ----------
# 检查目录是否存在
if [ ! -d "$SCRIPT_DIR" ]; then
    log_message "ERROR: Directory $SCRIPT_DIR does not exist"
    exit 1
fi

# 收集所有匹配的脚本文件
mapfile -t scripts < <(find "$SCRIPT_DIR" -maxdepth 1 -type f -name "$PATTERN" | sort)
if [ ${#scripts[@]} -eq 0 ]; then
    log_message "No scripts found matching pattern $PATTERN in $SCRIPT_DIR"
    exit 0
fi

log_message "Starting monitoring of ${#scripts[@]} scripts from $SCRIPT_DIR"

# 启动所有脚本
for i in "${!scripts[@]}"; do
    start_script "${scripts[$i]}" "$i"
done

# 主监控循环：wait -n 阻塞等待任意子进程退出
while true; do
    wait -n
    exit_code=$?

    # 检查所有进程，重启已退出的
    for i in "${!pids[@]}"; do
        pid=${pids[$i]}
        if [ -n "$pid" ] && ! kill -0 "$pid" 2>/dev/null; then
            log_message "Script $i (${scripts[$i]}) exited with code $exit_code. Restarting..."
            start_script "${scripts[$i]}" "$i"
        fi
    done
done
