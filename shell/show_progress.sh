#!/bin/bash
##########################################################
# Copyright (C) 2024 wqshao All rights reserved.
#  File Name    : show_progress.sh
#  Author       : wqshao
#  Created Time : 2024-02-28 14:14:40
#  Description  :
##########################################################

# 获取当前时间戳（毫秒）
getCurrentTimeMillis() {
	echo $(date +%s%3N)
}

# 对小数四舍五入
round() {
	printf "%.0f" $1
}

# 获取当前光标位置
fetch_cursor_position() {
  local pos

  IFS='[;' read -p $'\e[6n' -d R -a pos -rs || echo "failed with error: $? ; ${pos[*]}"
  echo "${pos[1]} ${pos[2]}"
}

# 根据传入的完成比显示进度条
showProgress() {
	percentComplete=$1
	# 进度条长度
	barlen=$(tput cols|awk '{print $1-22}')
	# 已完成部分的长度
	completed=$(round $(echo "$barlen*$percentComplete"|bc))
	equals=$(printf "%0.s=" $(seq 1 $completed))
	equals=$(echo $equals|sed 's/=$/>/')

	spaces=""
	if [ $completed -lt $barlen ];then
		# 未完成部分的长度
		incomplete=$(($barlen - $completed))
		if [ $completed -eq 0 ];then
			let incomplete--
		fi
		spaces=$(printf "%0.s " $(seq 1 $incomplete))
	fi

  # 将完成比转换成百分数
  percentage=$(round $(echo "$percentComplete*100"|bc))
  # 计算耗时
  elapsed=$(echo "scale=1; ($(getCurrentTimeMillis)-${startTimeMillis})/1000"|bc)
  # 打印进度等信息

  #cur_pos=$(fetch_cursor_position)
  ##echo "cur_pos=$cur_pos"

  #rows=$(tput lines)
  #tput cup $((rows-1)) 0

  printf "\r进度 %4d%%[%s%s] in %0.1fs" $percentage $equals "$spaces" $elapsed
  if [ "$percentage" == "100" ];then
	  echo
  fi

  #tput cup $cur_pos
}

# 总任务数
total=30
# 当前完成的任务数
current=0
# 开始时间
startTimeMillis=$(getCurrentTimeMillis)

echo "start"

showProgress 0
while [ $current -lt $total ]; do
  # 模拟任务完成
  ((current++))
  # 计算完成比
  percentComplete=$(echo "scale=2;$current/$total"|bc)
  #echo "-- percentComplete=$percentComplete"
  showProgress $percentComplete
done

#QUEUE_FILE=task.queue
##
##(percentComplete=0
##while [ $current -lt $total ]; do
##	# 计算完成比
##	# 将光标指定到最后一行打印进度条
##
##	current=`head -1 $QUEUE_FILE`
##	#echo "current=[$current]"
##	if [ x$current == x"" ]; then
##		current=$old_current
##		continue
##	fi
##	old_current=$current
##	echo "current=$current"
##
##	percentComplete=$(echo "scale=2;$current/$total"|bc)
##	echo "-- percentComplete=$percentComplete"
##	showProgress $percentComplete
##	sed -i '1d' $QUEUE_FILE
##done)  &
#
##echo 1 > $QUEUE_FILE
#
#showProgress .10
#
#sleep 1
#echo "11111"
#echo 5 > $QUEUE_FILE
#showProgress .20
#
#sleep 1
#echo "222222"
#echo 10 > $QUEUE_FILE
#showProgress .40
#
#sleep 2
#echo "333333"
#echo 16 > $QUEUE_FILE
#showProgress .55
#
#sleep 1
#echo "444444"
#echo 20 > $QUEUE_FILE
#showProgress .67
#
#sleep 3
#echo "555555"
#echo 25 > $QUEUE_FILE
#showProgress .89
#
#sleep 3
#echo "666666"
#echo 30 > $QUEUE_FILE
#showProgress .100
#
#wait
#
#echo "over"
