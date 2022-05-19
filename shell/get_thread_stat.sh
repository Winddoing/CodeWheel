#!/bin/sh

## FOR USERS
##
## Usage: get_thread_stat.sh <process name or pid number>
##        Parse /proc/<pid>/statm to get each thread stat.
##        It's usefull for device on which ps/top can't show more.
##
## Eg.    $ get_thread_stat.sh 1
##        == No. 0 ==
##        comm: procd
##        pid: 1
##        stat: S
##        min page fault: 39763
##        max page fault: 26
##        user time: 188 jiffies
##        system time: 685 jiffies
##        num threads : 1
##        vsize size: 3162112 pages
##        rss size: 269 pages
##        running cpu: 1
##        prio: 20
##        nice: 0

if [ $# -lt 1 ]; then
    echo "Please tell me pid or process name"
    exit 1
fi

get_pid() {
    if $(echo $1 | grep -qE '^[0-9]+$'); then
        echo $1
    else
        pidof $1
    fi
}

pid="$(get_pid $1)"
tasks="$(ls /proc/${pid}/task)"
cnt=0
for task in ${tasks}
do
    echo "== No. ${cnt} =="
    let cnt++

    cat /proc/${pid}/task/${task}/stat | awk -F '[ |(|)]' '{
        printf "comm: %s\n", $3;
        printf "pid: %d\n", $1;
        printf "stat: %s\n", $5;
        printf "min page fault: %d\n", $12;
        printf "max page fault: %d\n", $14;
        printf "user time: %d jiffies\n", $16;
        printf "system time: %d jiffies\n", $17;
        printf "num threads : %d\n", $22;
        printf "vsize size: %d pages\n", $25;
        printf "rss size: %d pages\n", $26;
        printf "running cpu: %d\n", $41;
        printf "prio: %d\n", $20;
        printf "nice: %d\n", $21;
        printf "\n";
    }'
done
