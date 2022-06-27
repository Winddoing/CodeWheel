#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : singleton_pattern.sh
#  Author       : wqshao
#  Created Time : 2022-06-27 19:25:18
#  Description  :
##########################################################

# 此函数用于获取不到锁时主动退出
activate_exit(){
    echo "`date +'%Y-%m-%d %H:%M:%S'`--error. get lock fail. there is other instance running. will be exit."
    exit 1
}

# 此函数用于申请锁
get_lock(){
    lock_file_name="/tmp/`basename $0`.pid"
    # exec 6<>${lock_file_name}，即以6作为lock_file_name的文件描述符（file descriptor number）
    # 6是随便取的一个数值，但不要是0/1/2，也不要太大（不要太太包含不能使用$$，$$值可能会比较大）
    # 不用担心如test.sh和test1.sh都使用
    exec 6<>${lock_file_name}
    # 如果获取不到锁，flock语句就为假，就会执行||后的activate_exit
    # 引入一个activate_exit函数的原因是||后不知道怎么写多个命令
    flock -n 6 || activate_exit
    # 如果没有执行activate_exit，那么程序就可以继续执行
    echo "`date +'%Y-%m-%d %H:%M:%S'`--ok. get lock success. there is not any other instance running."
    # 将当前获取锁的进程id写入文件
    echo "$$">&6

    # 设置监听信号
    # 当进程因这些信号致使进程中断时，最后仍要释放锁。类似java等中的final
    # 这个其实不需要，因为进程结束时fd会自动关闭
    # trap 'release_lock && activate_exit "1002" "break by some signal."' 1 2 3 9 15
}

# 程序主要逻辑
exec_main_logic(){
  echo "you can code your main logic in this function."
  # 这个sleep只是为了用于演示，替换成自己的代码即可
  sleep 30
}

# 程序主体逻辑
main(){
  # 获取锁
  get_lock $@
  # 程序主要逻辑
  exec_main_logic
}

main $@
