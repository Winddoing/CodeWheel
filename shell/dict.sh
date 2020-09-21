#!/bin/bash
##########################################################
# File Name		: dict.sh
# Author		: winddoing
# Created Time	: 2020年09月18日 星期五 17时53分26秒
# Description	:
##########################################################

declare -A dic

aaa=111
bbb=222
ccc=333

dic=([key1]="$aaa" [key2]="$bbb" \
    [key3]="$ccc")


for key in $(echo ${!dic[*]})
do
    echo "$key : ${dic[$key]}"
done

#打印指定key的value
echo ${dic["key1"]}
#打印所有key值
echo ${!dic[*]}
#打印所有value
echo ${dic[*]}
#字典添加一个新元素
dic+=（[key4]="value4"）
dic+=（[key5]="value5"）

#遍历key值
for key in $(echo ${!dic[*]})
do
    echo "$key : ${dic[$key]}"
done

echo "shell定义数组"

#数组
list=("value1" "value2" "value3")
#打印指定下标
echo ${list[1]}
#打印所有下标
echo ${!list[*]}
#打印数组下标
echo ${list[*]}
#数组增加一个元素
list=("${list[@]}" "value3")

#按序号遍历
for i in "${!arr[@]}"; do 
    printf "%s\t%s\n" "$i" "${arr[$i]}"
done

#按数据遍历
for NUM in ${ARR[*]}
do
    echo $NUM
done
