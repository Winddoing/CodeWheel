##########################################################
# File Name		: cpu.sh
# Author		: winddoing
# Created Time	: 2018年12月25日 星期二 11时19分11秒
# Description	:
##########################################################
#!/bin/bash

interval=3
cpu_num=`cat /proc/stat | grep cpu[0-9] -c`

start_idle=()
start_total=()
cpu_rate=()

cpu_rate_file=./`hostname`_cpu_rate.csv
if [ -f ${cpu_rate_file} ]; then
    mv ${cpu_rate_file} ${cpu_rate_file}.`date +%m_%d-%H_%M_%S`.bak
fi
for((i=0;i<${cpu_num};i++))
{
    echo -n "cpu$i," >> ${cpu_rate_file}
}

echo -n "cpu_avg" >> ${cpu_rate_file}
echo "" >> ${cpu_rate_file}

while(true)
do
    for((i=0;i<${cpu_num};i++))
        {
            start=$(cat /proc/stat | grep "cpu$i" | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}')
            start_idle[$i]=$(echo ${start} | awk '{print $4}')
            start_total[$i]=$(echo ${start} | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}')
        }
        start=$(cat /proc/stat | grep "cpu " | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}')
        start_idle[${cpu_num}]=$(echo ${start} | awk '{print $4}')
        start_total[${cpu_num}]=$(echo ${start} | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}')
        sleep ${interval}
        for((i=0;i<${cpu_num};i++))
            {
            end=$(cat /proc/stat | grep "cpu$i" | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}')
            end_idle=$(echo ${end} | awk '{print $4}')
            end_total=$(echo ${end} | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}')
            idle=`expr ${end_idle} - ${start_idle[$i]}`
            total=`expr ${end_total} - ${start_total[$i]}`
            idle_normal=`expr ${idle} \* 100`
            cpu_usage=`expr ${idle_normal} / ${total}`
            cpu_rate[$i]=`expr 100 - ${cpu_usage}`
            echo "The CPU$i Rate : ${cpu_rate[$i]}%"
            echo -n "${cpu_rate[$i]}," >> ${cpu_rate_file}
        }
    end=$(cat /proc/stat | grep "cpu " | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}')
    end_idle=$(echo ${end} | awk '{print $4}')
    end_total=$(echo ${end} | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}')
    idle=`expr ${end_idle} - ${start_idle[$i]}`
    total=`expr ${end_total} - ${start_total[$i]}`
    idle_normal=`expr ${idle} \* 100`
    cpu_usage=`expr ${idle_normal} / ${total}`
    cpu_rate[${cpu_num}]=`expr 100 - ${cpu_usage}`
    echo "The Average CPU Rate : ${cpu_rate[${cpu_num}]}%"
    echo -n "${cpu_rate[${cpu_num}]}" >> ${cpu_rate_file}
    echo "------------------"
    echo "" >> ${cpu_rate_file}
done
