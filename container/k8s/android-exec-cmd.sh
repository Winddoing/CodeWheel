#!/bin/bash

function usage() {
	cat <<EOF
Usage: `basename $0` COMMAND OPTIONS 

Commands:
    exec  执行虚拟机shell命令，使用方法：
          exec -vl <虚拟机列表> -cmd <shell命令> [-p]
             
Options:
    -vl, --vm-list    string    虚拟机列表，特定值“all”表示集群所有虚拟机
                                通用格式为：<服务器1>:<虚拟机编号>;<服务器2>:<虚拟机编号>;...
				其中：
			            <服务器>为服务器主机名，可用“-”表示当前主机
                                    <虚拟机编号>的表示方法为：“1,2,3,4,5” 或 “1-5” 或 “1,2,3,4-5” 或 “all”
                                注意：如果同时操作多台服务器，则选项值必须用双引号括起来
                                示例："bg-node-0:1,2,3;bg-node-1:1-3;bg-node-2:1,2,3,4-5;bg-node-3:all"
    -cmd              string    需要执行的虚拟机shell命令
    -p                          并发模式，该模式下所有虚拟机将会同时执行shell命令
                                注意：此模式下虚拟机执行命令时的输出信息将不会打印到控制台，而是保存到对应的log文件                       

使用说明：
    1. 该脚本必须在集群master节点上运行。
 
EOF
exit
}

function info() {
	echo -e "\033[34m[$(date +%F_%T)] INFO: $@\033[0m"
}

function warn() {
	echo -e "\033[33m[$(date +%F_%T)] WARN: $@\033[0m"
}

function error() {
	echo -e "\033[31m[$(date +%F_%T)] ERROR: $@\033[0m" >&2
	exit -1
}

function assert() {
	[[ $1 != 0 ]] && error "$2"
	return 0
}

function ok() {
	echo -e "\033[32m[$(date +%F_%T)] SUCCESS: $@\033[0m"
}

CMD=
VMS=
VM_CMD=
PARALLEL=
LOG_DIR=log
declare -A NODES_VMS_MAP=()
K8S_NODE_NAME=`echo $(hostname) | tr 'A-Z' 'a-z'`

function process_params() {
	[[ $1 != "exec" ]] ||
	[[ $1 == "help" ]] || 
	[[ $1 == "-h" ]] || 
	[[ $1 == "--help" ]] && usage
	CMD=$1
	shift
  
	while [[ $# -gt 0 ]]; do
		case $1 in
			-vl|--vm-list)
				VMS=$2
				shift
				;;
		    	-cmd)
				VM_CMD=$2
				shift
				;;
			-p)
				PARALLEL=1
				;;
			*)
				error "无效的命令选项: $1"
		esac
		shift
	done

	[[ $CMD == "exec" && ($VMS == "" || $VM_CMD == "") ]] && usage	

	if [[ $VMS == "all" ]]; then
		local nodes=`kubectl get node | grep -v -w NAME | awk '{print $1}'`
		for node in $nodes; do
			local vms=`kubectl get statefulset -n redroid-android | grep $node | awk '{print $1}'`
			local vmIdxSet=
			for n in $vms; do
				vmIdxSet="$vmIdxSet ${n##*-}"
			done

			NODES_VMS_MAP[$node]=$vmIdxSet
		done
	else
		local tmpVms=${VMS//;/ }
		for nodeVms in $tmpVms; do
			local node=${nodeVms%%:*}
			if [[ $node =~ ^[0-9a-zA-Z_-]+$ ]]; then
				[[ $node == "-" ]] && node=$K8S_NODE_NAME
				kubectl get node $node > /dev/null
				if [[ $? != 0 ]]; then
					error "无法获取服务器($node)的信息，请确认该服务器已经加入集群"
				fi
			else
				error "无效的服务器名：$node"
			fi

			for k in ${!NODES_VMS_MAP[@]}; do
				[[ $k == $node ]] && error "重复的服务器：$node"
			done

			local vmIdxSet=
			local vmIdxs=${nodeVms##*:}
			vmIdxs=${vmIdxs//,/ }
			if [[ $vmIdxs != "all" ]]; then
				for idx in $vmIdxs; do
					if [[ $idx =~ ^[1-9][0-9]{0,}$ ]]; then
						vmIdxSet="$vmIdxSet `printf "%03d" $idx`"
					elif [[ $idx =~ ^[1-9][0-9]{0,}-[1-9][0-9]{0,}$ ]]; then
						local startIdx=${idx%%-*}
						local endIdx=${idx##*-}
						while [[ $startIdx -le $endIdx ]]; do
							vmIdxSet="$vmIdxSet `printf "%03d" $startIdx`"
							let startIdx++
						done
					else
						error "无效的虚拟机编号：$VMS"
					fi
				done
			fi

			local vmNames=`kubectl get statefulset -n redroid-android | grep $node | awk '{print $1}'`
			if [[ $vmIdxs == "all" ]]; then
				for n in $vmNames; do
					vmIdxSet="$vmIdxSet ${n##*-}"
				done
			else
				for idx in $vmIdxSet; do
					local flg=0
					for n in $vmNames; do
						d=${n##*-}
						[[ $idx == $d ]] && flg=1 
					done

					[[ $flg == 0 ]] && error "服务器（$node）上不存在$idx号虚拟机"
				done
			fi

			NODES_VMS_MAP[$node]=$vmIdxSet	
		done
	fi

	# 打印虚拟机MAP映射表 
	# for k in ${!NODES_VMS_MAP[@]}; do
	# 	warn "$k:${NODES_VMS_MAP[$k]}"
	# done
}

function vm_exec_cmd() {
	local vmName=$1
	local vmLogFile=$LOG_DIR/${vmName}_$(date +%F_%T).log

	if [[ $PARALLEL == 1 ]]; then
		kubectl exec $vmName-0 -n redroid-android -- sh -c "$VM_CMD" &> $vmLogFile
		if [[ $? != 0 ]]; then
			warn "$vmName: 命令执行失败，详见日志文件：$vmLogFile"
		else
			ok "$vmName: 命令执行成功，详见日志文件：$vmLogFile"
		fi
	else
		info "$vmName:"
		kubectl exec $vmName-0 -n redroid-android -- sh -c "$VM_CMD"
	fi
}

function vms_exec_cmd() {
	for node in ${!NODES_VMS_MAP[@]}; do
		info "========================================== $node =========================================="
		local shutdownVms=`kubectl get statefulset -n redroid-android | grep $node | grep 0/0 | awk '{print $1}'`
		for vmIdx in ${NODES_VMS_MAP[$node]}; do
			local vmName=$node-$vmIdx
			local flg=0
			for v in $shutdownVms; do
				[[ $v == $vmName ]] && flg=1 
			done
			if [[ $flg == 1 ]]; then
				warn "$vmName: 虚拟机已关机，无法执行shell命令"
			else
				if [[ $PARALLEL == 1 ]]; then
					vm_exec_cmd "$vmName" &
				else
					vm_exec_cmd "$vmName"	
				fi
			fi
		done
		wait
		echo ""
	done
}

function main() {
   	process_params "$@"

	if [[ ! -e $LOG_DIR ]]; then
		mkdir -p $LOG_DIR
	else
		rm -rf $LOG_DIR/*
	fi
	
    	if [[ $CMD == "exec" ]]; then
		vms_exec_cmd
    	fi
}
main "$@"
exit $?



