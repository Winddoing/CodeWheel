#!/bin/bash
##########################################################
# Copyright (C) 2026 wqshao All rights reserved.
#  File Name    : process_monitor.sh
#  Author       : wqshao
#  Created Time : 2026-03-26 15:15:42
#  Description  :
##########################################################
# 监控多个进程，退出后自动重启（事件驱动，无轮询）

# List of commands to monitor (one per line)
commands=(
    "sleep 10"          # Example: exits after 10 seconds
    "sleep 20"          # Example: exits after 20 seconds
    #"python3 -m http.server 8000"   # Example: start an HTTP service
)

# Global array to store PIDs
declare -a pids

# Log function: print to terminal and send to syslog
log_message() {
    local msg="$1"
    local timestamp=$(date '+%H:%M:%S')
    echo "[$timestamp] $msg"
    logger -t "monitor.sh" "$msg"
}

# Start a process and record its PID
# Arguments: $1 = command, $2 = index (for identification)
start_process() {
    local cmd="$1"
    local idx="$2"
    eval "$cmd" &
    local pid=$!
    pids[$idx]=$pid
    log_message "Started process $idx: $cmd (PID $pid)"
}

# Cleanup: kill all monitored child processes
cleanup() {
    log_message "Received exit signal, cleaning up all child processes..."
    for pid in "${pids[@]}"; do
        if kill -0 "$pid" 2>/dev/null; then
            kill "$pid" 2>/dev/null
            wait "$pid" 2>/dev/null
        fi
    done
    exit 0
}

# Trap signals to ensure cleanup
trap cleanup SIGINT SIGTERM EXIT

# Start all initial processes
log_message "Starting monitoring of ${#commands[@]} processes..."
for i in "${!commands[@]}"; do
    start_process "${commands[$i]}" "$i"
done

# Main monitoring loop: wait -n blocks until any child exits
while true; do
    wait -n
    exit_code=$?

    # Check all processes, restart any that have exited
    for i in "${!pids[@]}"; do
        pid=${pids[$i]}
        if ! kill -0 "$pid" 2>/dev/null; then
            log_message "Process $i (old PID $pid) exited with code $exit_code. Restarting..."
            start_process "${commands[$i]}" "$i"
        fi
    done
done
