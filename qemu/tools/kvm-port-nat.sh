#!/bin/bash
#set -x

CMD=
SRC_PORTS=
DST_IP=
DST_PORTS=
PROTOCOL=
SRC_START_PORT=
SRC_END_PORT=
DST_START_PORT=
DST_END_PORT=

function error() {
	echo "ERROR: $@" >&2
}

function fatal() {
	echo "FATAL: $@" >&2
	exit -1
}

function assert() {
	[[ $1 != 0 ]] && fatal "$2"
	return 0
}

function usage() {
	echo "usage:"
	echo "  $0 get|del <src_port_range>"
	echo "  $0 add <src_port_range> <dst_ip> <dst_port_range> tcp|udp|all"
	exit -1
}

function process_args() {
        CMD=$1
        SRC_PORTS=$2
        DST_IP=$3
        DST_PORTS=$4
        PROTOCOL=$5
        
        [[ $CMD != get && $CMD != add && $CMD != del ]] && usage

        if [[ $CMD == get || $CMD == add || $CMD == del ]]; then
                [[ $SRC_PORTS == "" ]] && usage
                if [[ $SRC_PORTS =~ ^[1-9][0-9]{0,}$ ]]; then
                        SRC_START_PORT=$SRC_PORTS
                        SRC_END_PORT=$SRC_PORTS
                        [[ $SRC_PORTS -gt 65535 ]] && fatal "invalid <src_port_range>: $SRC_PORTS"
                elif [[ $SRC_PORTS =~ ^[1-9][0-9]{0,}-[1-9][0-9]{0,}$ ]]; then
                        SRC_START_PORT=${SRC_PORTS%%-*}
                        SRC_END_PORT=${SRC_PORTS##*-}
                        [[ $SRC_START_PORT -gt 65535 ]] && fatal "invalid <src_port_range>: $SRC_PORTS"
                        [[ $SRC_START_PORT -gt $SRC_END_PORT ]] && fatal "invalid <src_port_range>: $SRC_PORTS"
                else
                        fatal "invalid <src_port_range>: $SRC_PORTS"
                fi               
        fi
        
        if [[ $CMD == add ]]; then
                [[ $DST_IP == "" ]] && usage
                [[ ! $DST_IP =~ ^[1-9][0-9]{0,}.[1-9][0-9]{0,}.[1-9][0-9]{0,}.[1-9][0-9]{0,}$ ]] && fatal "invalid <dst_ip>: $DST_IP"  

                [[ $DST_PORTS == "" ]] && usage
                if [[ $DST_PORTS =~ ^[1-9][0-9]{0,}$ ]]; then
                        DST_START_PORT=$DST_PORTS
                        DST_END_PORT=$DST_PORTS
                        [[ $DST_PORTS -gt 65535 ]] && fatal "invalid <dst_port_range>: $DST_PORTS"
                elif [[ $DST_PORTS =~ ^[1-9][0-9]{0,}-[1-9][0-9]{0,}$ ]]; then
                        DST_START_PORT=${DST_PORTS%%-*}
                        DST_END_PORT=${DST_PORTS##*-}
                        [[ $DST_START_PORT -gt 65535 ]] && fatal "invalid <dst_port_range>: $DST_PORTS"
                        [[ $DST_START_PORT -gt $DST_END_PORT ]] && fatal "invalid <dst_port_range>: $DST_PORTS"
                else
                        fatal "invalid <dst_port_range>: $DST_PORTS"
                fi               

                srcPortNum=$(($SRC_END_PORT-$SRC_START_PORT))
                dstPortNum=$(($DST_END_PORT-$DST_START_PORT))
                [[ $srcPortNum != $dstPortNum ]] && fatal "<src_port_range> isn't match with <dst_port_range>"

                [[ $PROTOCOL != tcp && $PROTOCOL != udp && $PROTOCOL != all ]] && usage
        fi
}

function get_ports() {
        ipt=`iptables-save`
        for(( i=$SRC_START_PORT; i <= $SRC_END_PORT; i++ )); do
                v=`echo "$ipt" | grep '^-A' | grep -w $i`
                if [[ $? == 0 ]]; then
                        echo "$v"
                fi
        done        
}

function add_ports() {
        iptables -t nat -N QEMU_KVM &> /dev/null
        [[ $? == 0 ]] && iptables -t nat -I PREROUTING -j QEMU_KVM 

        failedCount=0
        j=$(($DST_START_PORT-1))
        for(( i=$SRC_START_PORT; i <= $SRC_END_PORT; i++ )); do
                let j++

                v=`iptables-save | grep '^-A' | grep -w $i`
                if [[ $? == 0 ]]; then
                        error "port($i) nat rule already exist"
                        let failedCount++
                else
                        if [[ $PROTOCOL == tcp ]]; then
                                iptables -t nat -I QEMU_KVM -p tcp --dport $i -j DNAT --to-destination $DST_IP:$j
                                if [[ $? != 0 ]]; then
                                        error "failed to add port($i/tcp) nat rule"
                                        let failedCount++
                                fi
                        elif [[ $PROTOCOL == udp ]]; then
                                iptables -t nat -I QEMU_KVM -p udp --dport $i -j DNAT --to-destination $DST_IP:$j
                                if [[ $? != 0 ]]; then
                                        error "failed to add port($i/udp) nat rule"
                                        let failedCount++
                                fi
                        else
                                iptables -t nat -I QEMU_KVM -p tcp --dport $i -j DNAT --to-destination $DST_IP:$j
                                if [[ $? != 0 ]]; then
                                        error "failed to add port($i/tcp) nat rule"
                                        let failedCount++
                                else
                                        iptables -t nat -I QEMU_KVM -p udp --dport $i -j DNAT --to-destination $DST_IP:$j
                                        if [[ $? != 0 ]]; then
                                                error "failed to add port($i/udp) nat rule"
                                                let failedCount++
                                        fi
                                fi
                        fi
                fi
        done        
        [[ $failedCount != 0 ]] && echo "================== failed ports: $failedCount =================="
}

function del_ports() {
        ipt=`iptables-save`
        for(( i=$SRC_START_PORT; i <= $SRC_END_PORT; i++ )); do
                v=`echo "$ipt" | grep '^-A' | grep -w $i`
                if [[ $? == 0 ]]; then
                        echo "$v" | while read line; do
                                r=`echo "$line" | sed 's/-A/iptables -t nat -D/'`
                                $r
                                [[ $? != 0 ]] && error "failed to delete port($i) nat rule"
                        done
                fi
        done        
}

function main() {
        ls /root &> /dev/null
        [[ $? != 0 ]] && fatal "please use sudo"

	process_args "$@"
        
        if [[ $CMD == get ]]; then
                get_ports
        elif [[ $CMD == add ]]; then
                add_ports
        elif [[ $CMD == del ]]; then
                del_ports
        fi
}
main "$@"
exit $?
