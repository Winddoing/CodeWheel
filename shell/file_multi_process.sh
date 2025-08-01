#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : file_multi_process.sh
#  Author       : wqshao
#  Created Time : 2025-05-16 17:21:22
#  Description  :
##########################################################

#!/bin/bash

# 查找所有PNG和JPG文件，使用NUL分隔符处理特殊字符
readarray -d '' -t files < <(find . -type f \( -name "*.png" -o -name "*.jpg" \) -print0)

total=${#files[@]}
if [[ $total -eq 0 ]]; then
    echo "未找到任何文件。"
    exit 0
fi

# 计算分组参数
group_size=$((total / 4))
remainder=$((total % 4))

start=0
pids=()

# 启动四个并行进程
for ((i=0; i<4; i++)); do
    # 确定当前组的大小
    if [[ $i -lt $remainder ]]; then
        this_group_size=$((group_size + 1))
    else
        this_group_size=$group_size
    fi

    # 提取当前组的文件
    current_group=("${files[@]:start:this_group_size}")
    ((start += this_group_size))

    # 在子shell中处理当前组文件（后台执行）
    (
        for file in "${current_group[@]}"; do
            echo "处理文件: $file"
            # 此处替换为实际处理命令，例如：
            # convert "$file" ... 或其他操作
        done
    ) &
    pids+=($!)
done

# 等待所有后台进程完成
for pid in "${pids[@]}"; do
    wait "$pid"
done

echo "全部处理完成。"
