#!/bin/sh

## FOR USERS
##
## Usage: get_rss.sh <process name or pid number>
##        Parse /proc/<pid>/statm to get VSS and RSS.
##        It's usefull for device on which ps/top can't show RSS.
##
## Eg.    $ get_rss.sh 1
##        PID  VSS    RSS    CMD
##        1    3MB    1MB    /sbin/procd ...

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

echo -e "PID\tVSS\tRSS\tCMD"
for p in $@
do
    pid="$(get_pid $p)"
    cmd="$(cat /proc/${pid}/cmdline 2> /dev/null | \
        awk '{for (i = 1; i <= NF; i++) {printf $1 " "}}')"
    if [ -f "/proc/${pid}/statm" ]; then
        echo -en "${pid}\t"
        awk '{printf "%dMB\t%dMB", $1 * 4 / 1024, $2 * 4 / 1024}' /proc/${pid}/statm
        printf "\t%.15s...\n" "${cmd}"
    fi
done
