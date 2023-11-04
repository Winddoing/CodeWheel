#!/bin/bash

######################################################### 虚拟机配置 #######################################################
# 虚拟机登录方式: app - 通过手机app客户端登录， h5 - 通过web端h5页面登录
ANDROID_LOGIN_METHOD=app
#虚拟机镜像名称
#如需远程从镜像仓库下载，镜像名称格式必须为：<镜像仓库服务地址>/<镜像仓库路径>:<镜像tag>
#远程镜像名称示例：registry.bg.com/vmi/android:latest
ANDROID_7_IMAGE="registry.bg.com/vmi/android_for_suqi:kp916_v1.3.6_useEncodeCard_SoftEncode_redroidv1.2.6_debug"
ANDROID_9_IMAGE="registry.bg.com/develop/android_for_develop:Ampere_UseNetintT408Card_SuqiStream_redroid-9.0.0_r3.2.3"
ANDROID_11_IMAGE="registry.bg.com/develop/android_for_develop:redroid-11.0.0"
ANDROID_12_IMAGE="registry.bg.com/develop/android_for_develop:redroid-12.0.0"
#虚拟机镜像下载凭证，当创建虚拟机的镜像在服务器本地不存在时将自动使用该凭证从对应的镜像仓库下载
REGISTRY_SECRET=default
#是否启用虚拟机cpu绑核策略，0-禁止  1-启用
CPU_BIND_ENABLE=1
#虚拟机cpu绑核模式
# 0 - 整体规划模式（系统内置策略-虚拟机分配的cpu根据对应的计算公式确定，该策略有可能会跨numa分配cpu）  
# 1 - 动态分配模式（系统内置策略-虚拟机分配的cpu从空闲cpu池中动态分配，该策略不会跨numa分配cpu）  
# 2 - 手动分配模式（用户自定义策略-需手动指定每台虚拟机绑定的cpu核心）
CPU_BIND_MODE=1
#cpu绑核模式下虚拟机container与redroid是否共享cpu核心（仅安卓7有效）
# 0 - 独立模式，虚拟机container与redroid独立绑cpu核
# 1 - 共享模式，虚拟机redroid与container绑同样的cpu核
CPU_SHARE_ENABLE=0
#给虚拟机container分配的cpu核数，在cpu绑核模式为0,1时必须为大于0的整型数值，为2时可以是以0.5为阶梯的浮点型数值，例如1.5，2.5
CONTAINER_CPUS=6
#给虚拟机redroid服务分配的cpu核数,手动模式下需将该参数赋值为空（仅安卓7有效）
redroid_CPUS=2
#手动绑核模式下给虚拟机container所绑定的cpu核，
#多核之间以“,”分隔，如"0,1"表示虚拟机container绑定的cpu为cpu0和cpu1，
#自动模式下需将该参数赋值为空 
CONTAINER_CPU_SETS=
#手动绑核模式下给虚拟机redroid所绑定的cpu核，若CPU_SHARE_ENABLE=1，
#则该值强制等于CONTAINER_CPU_SETS，非手动模式下需将该参数赋值为空（仅安卓7有效）
redroid_CPU_SETS=
#虚拟机绑核是否按分组绑定，该参数仅在cpu绑核为整体规划模式且服务器为鲲鹏920时才生效
CPU_GROUP_ENABLE_FOR_KP920=0
#是否使用显卡, 0-禁止  1-使用
GPU_ENABLE=1
#显卡分配模式，0-自动模式  1-手动模式
GPU_ALLOC_MODE=0
#显卡手动分配模式下指定的显卡ID，为虚拟机所在服务器上显卡的BusID，例如: 000a:11:00.0
GPU_ID=
#是否使用编码卡，0-禁止  1-启用
ENC_ENABLE=0
#编码卡分别配模式，0-自动模式  1-手动模式
ENC_ALLOC_MODE=0
#编码卡手动分配模式下指定的编码卡ID，其值为虚拟机所在服务器上编码卡的BusID，例如: 000a:01:00.0
ENC_ID=
#编码卡手动分配模式下指定的编码卡序号（仅安卓7有效）
#各服务器上的编码卡信息文件所在路径为:/opt/redroid/device-plugin/encode_card_infos，
#其json格式数据中的DeviceIdx字段即表示为ENC_IDX的值，若不使用编码卡请将其赋值为空。
ENC_IDX=
#内存大小，单位MB
MEMORY=8192
#2MB单位大页内存大小，单位MB（当前仅使用浪潮编码卡时才需分配，其他情况应赋值为空）
HUGE_PAGE_2MB=
#磁盘大小，单位GB
DISK_SIZE=50
#屏幕分辨率宽度
SCREEN_WIDTH=1080
#屏幕分辨率高度
SCREEN_HEIGHT=1920
#屏幕显示密度
SCREEN_DPI=480
#屏幕刷新率
SCREEN_FPS=30
#屏幕横向显示使能
SCREEN_ROTATE=0
#虚拟机远程桌面是否使用vnc协议（仅安卓7支持VNC）, 0-禁止  1-启用
VNC_ENABLE=0
#同一主机上的虚拟机VNC起始端口号（虚拟机vnc端口 = 虚拟机VNC起始端口号 + 虚拟机编号 - 1）
VNC_BASE_PORT=64000
#同一主机上的虚拟机ADB起始端口号（虚拟机adb端口 = 虚拟机ADB起始端口号 + 虚拟机编号 - 1）
ADB_BASE_PORT=63000
#虚拟机串流服务映射到宿主机的起始端口（虚拟机串流宿主机端口 = 虚拟机串流宿主机起始端口号 + (虚拟机编号-1)*虚拟机串流服务端口数量）
RTSP_HOST_BASE_PORT=60000
#虚拟机串流服务容器内部的起始端口
RTSP_CONTAINER_BASE_PORT=60000
#虚拟机串流服务的端口数目
RTSP_PORT_NUM=10
#虚拟机MAC地址自定义规则：公司编号（3字节）+  集群编号（1字节）+ 主机编号（1字节）+ 虚拟机编号（1字节）
MAC_COMPANY="5E-66-88"  #公司编号，3字节表示
MAC_CLUSTER="00"        #集群编号（地区编号），1字节表示
#redroid日志级别（仅安卓7有效）
redroid_LOG_LEVEL=warning
####################################################################################################################
####################################################################################################################
####################################################################################################################
####################################################################################################################
NODE_SSH_PORT=2022
ANDROID_VERSION=
OS_TYPE=
CMD=
CMD_CN=
VMS=
VM_IMAGE=
LOG_DIR=log
declare -A NODES_VMS_MAP=()
VM_DATA_DELETE_FLG=
VM_MANAGE_DIR=/tmp/android-vm-manage.$USER
ALL_VM_INFO_FILE=$VM_MANAGE_DIR/all_vm.info
ALL_NODE_INFO_FILE=$VM_MANAGE_DIR/all_node.info
NODE_ANDROID_ENV_SH=/opt/redroid/android-env/android-env.sh
K8S_NODE_NAME=`echo $(hostname) | tr 'A-Z' 'a-z'`

function get_os_info() {
        cat /etc/issue 2> /dev/null | grep Ubuntu > /dev/null
        if [[ $? == 0 ]]; then
                OS_TYPE=ubuntu
        else
                cat /etc/redhat-release 2> /dev/null | grep CentOS > /dev/null
                if [[ $? == 0 ]]; then
                        OS_TYPE=centos
                        which yum-config-manager &> /dev/null
                        if [[ $? != 0 ]]; then
                                yum install -y yum-utils
                                [[ $? != 0 ]] && fatal "安装yum-utils失败"
                        fi
                else
                        fatal "不支持的操作系统类型，目前仅支持ubuntu和centos"
                fi
        fi
}

function process_vm_config() {
        [[ $GPU_ENABLE == 0 ]] && warn "您没有设置虚拟机显卡使能，将默认使用服务器CPU软渲染，性能可能较低"
        [[ $GPU_ALLOC_MODE == 1 ]] && warn "您设置了虚拟机显卡分配模式为手动，请确保指定的显卡存在于虚拟机所在的服务器上"
        [[ $ENC_ENABLE == 0 ]] && warn "您没有设置编码卡使能，将默认使用服务器CPU软编码，性能可能会受影响"
        [[ $ENC_ALLOC_MODE == 1 ]] && warn "您设置了虚拟机编码卡分配模式为手动，请确保指定的编码卡存在于虚拟机所在的服务器上"
        [[ $DISK_SIZE =~ "GB" ]] && fatal "无效的参数值DISK_SIZE：$DISK_SIZE"

        [[ $CPU_BIND_ENABLE == 1 && $CONTAINER_CPUS == "" ]] && fatal "绑核模式下参数CONTAINER_CPUS能为空"
        [[ $GPU_ALLOC_MODE == 0 && $GPU_ID != "" ]] && fatal "显卡自动分配模式下，GPU_ID不能赋值"
        [[ $ENC_ALLOC_MODE == 0 && $ENC_ID != "" ]] && fatal "编码卡自动赋值模式下，ENC_ID不能赋值"
        [[ $GPU_ALLOC_MODE == 1 && $GPU_ID == "" ]] && fatal "显卡手动分配模式下请设置虚拟机显卡ID"
        [[ $ENC_ALLOC_MODE == 1 && $ENC_ID == "" ]] && fatal "编码卡手动分配模式下请设置虚拟机编码卡ID"

        if [[ $CPU_BIND_MODE == 2 ]]; then
                echo $CONTAINER_CPUS | grep '^[1-9][0-9]\{0,\}\(\.5\)\?$' > /dev/null
                [[ $? != 0 ]] && fatal "手动绑核模式下参数CONTAINER_CPUS只能为大于0的整型数值或以0.5为阶梯的浮点型数值"
                
                [[ $CONTAINER_CPU_SETS == "" ]] && fatal "参数值CONTAINER_CPU_SETS不能为空"
                [[ $ANDROID_VERSION == 7 && $redroid_CPU_SETS == "" && $CPU_SHARE_ENABLE == 0 ]] && fatal "手动且独立绑核模式下参数redroid_CPU_SETS必须赋值"

                [[ $ANDROID_VERSION == 7 && $redroid_CPUS != "" ]] && fatal "手动绑核模式下参数redroid_CPUS不能赋值"

                [[ $CONTAINER_CPU_SETS =~ ^[0-9,]{1,}$ ]] || fatal "无效的参数值CONTAINER_CPU_SETS：$CONTAINER_CPU_SETS"
                if [[ $ANDROID_VERSION == 7 && $CPU_SHARE_ENABLE == 0 ]]; then
                        [[ $redroid_CPU_SETS =~ ^[0-9,]{1,}$ ]] || fatal "无效的参数值CONTAINER_CPU_SETS：$redroid_CPU_SETS"
                fi
                n=${CONTAINER_CPU_SETS//,/ }; a=($n)
                [[ ${#a[@]} != $CONTAINER_CPUS ]] && warn "您给虚拟机分配和绑定的CPU核数目不一致，请确认是否为正常需求"
        else
                echo $CONTAINER_CPUS | grep '^[1-9][0-9]\{0,\}$' > /dev/null
                [[ $? != 0 ]] && fatal "非手动绑核模式下参数CONTAINER_CPUS只能为大于0的整型数值"

                [[ $CONTAINER_CPU_SETS != "" ]] && fatal "非手动绑核模式下参数CONTAINER_CPU_SETS不能赋值"
                [[ $ANDROID_VERSION == 7 && $redroid_CPU_SETS != "" ]] && fatal "非手动绑核模式下参数redroid_CPU_SETS不能赋值"

                [[ $ANDROID_VERSION == 7 && $redroid_CPUS == "" && $CPU_SHARE_ENABLE == 0 ]] && fatal "非手动且独立绑核模式下参数值redroid_CPUS必须赋值"
        fi

        if [[ $ANDROID_VERSION == 7 && $CPU_SHARE_ENABLE == 1 ]]; then
                [[ $redroid_CPUS != $CONTAINER_CPUS && $CPU_BIND_MODE != 2 ]] && fatal "非手动且共享绑核模式下参数值CONTAINER_CPUS必须与redroid_CPUS一致"
                [[ $redroid_CPU_SETS != $CONTAINER_CPU_SETS ]] && fatal "共享绑核模式下参数值CONTAINER_CPU_SETS必须与redroid_CPU_SETS一致"
        fi

        if [[ $ANDROID_VERSION != 7 ]]; then
                CPU_SHARE_ENABLE=0
                redroid_CPUS=0
                redroid_CPU_SETS=""        
                VNC_ENABLE=0
        fi

        echo ""
}

function usage() {
        cat <<EOF
Usage: `basename $0` COMMAND OPTIONS

Commands:
    view        查看服务器虚拟机统计信息，使用方法：
                view -nl <服务器列表>

    status      查看服务器虚拟机运行状态，使用方法：
                status -nl <服务器列表>

    inspect     查看指定虚拟机的静态配置模板，使用方法：
                inspect -vn <虚拟机名称>

    config      查看指定虚拟机的运行配置信息，使用方法：
                config -vn <虚拟机名称>

    create      创建（创建后自动启动）虚拟机，使用方法：
                create -vl <虚拟机列表>

    startup     启动虚拟机，使用方法：
                startup -vl <虚拟机列表>

    shutdown    关闭虚拟机，使用方法：
                shutdown -vl <虚拟机列表>

    reboot      重启虚拟机，使用方法：
                reboot -vl <虚拟机列表>

    upgrade     升级虚拟机，使用方法：
                upgrade -vl <虚拟机列表> -vi <虚拟机镜像>

    reset       重置虚拟机（恢复出厂），使用方法：
                reset -vl <虚拟机列表>

    delete      删除虚拟机，使用方法：
                delete -vl <虚拟机列表>


Options:
    -nl, --node-list        string    服务器列表，服务器之间以“,”分隔，“all”表示集群所有服务器，“-”表示当前服务器
    -vl, --vm-list          string    虚拟机列表，“all”表示集群所有虚拟机
                                      通用格式为：<服务器1>:<虚拟机编号>;<服务器2>:<虚拟机编号>;...
                                      其中：
                                          <服务器>为服务器主机名，“-”表示当前服务器
                                          <虚拟机编号>的表示方法为：“1,2,3,4,5” 或 “1-5” 或 “1,2,3,4-5” 或 “all”
                                      注意：如果同时操作多台服务器，则选项值必须用双引号括起来
                                      示例："bg-node-0:1,2,3;bg-node-1:1-3;bg-node-2:1,2,3,4-5;bg-node-3:all"
    -vn, --vm-name          string    虚拟机名称
    -vi, --vm-image         string    虚拟机镜像，格式为：<镜像仓库服务地址>/<镜像仓库名称>:<镜像tag>
                                      示例：“registry.bg.com/default/android:latest”


使用说明：
    1. 该脚本必须在集群master节点上运行。
    2. 请确保集群master节点可以通过ssh以root身份免密登录到集群所有服务器节点（包括master节点自身）。

EOF
exit
}

function error_debug_guide() {
        echo -e "======================================================================================================================================="
        echo -e "\033[33m提示：如虚拟机创建、开机、重启、重置和升级异常，可按如下步骤定位相关问题：\033[0m"
        cat <<EOF
1. 如果脚本很长时间一直卡住没反应，可通过“ctrl+c”强制退出，然后通过命令“kubectl get pod -n redroid-android”查看虚拟机pod的创建状态，
   如果虚拟机pod一直处于"Pending"状态，则运行命令“kubectl get node”查看虚拟机所在节点是否为“Ready”状态，若为“NotReady”状态，则登录到
   虚拟机所在节点执行命令“journalctl -u kubelet -f”观察kubelet服务的相关错误日志，若为“Ready”状态(且虚拟机为安卓7)则跳至步骤2，如果虚
   拟机pod一直处于"Init:0/1"状态，则跳至步骤3，都没有则跳至步骤4。
2. 登录到虚拟机所在节点，执行命令“service redroid-device-plugin status”查看该服务是否存在和运行正常，如不存在则进行安装，如运行异常
   则执行命令“journalctl -u redroid-device-plugin.service -f”查看服务的相关错误日志，如运行正常则跳至步骤4。
3. 登录到虚拟机所在节点，执行命令“service redroid-android-env status”查看该服务是否存在和运行正常，如不存在则进行安装，如运行异常则
   执行命令“journalctl -u redroid-android-env -f”查看服务的相关错误日志，如运行正常则跳至步骤4。
4. 如果脚本提示镜像拉取失败的错误，则登录到虚拟机所在节点，执行命令“docker images”查看服务器上存在的镜像，如果虚拟机镜像不存在请通过命
   令“docker pull <安卓镜像名称>”拉取，若命令执行失败且提示用户认证失败相关信息请通过命令“docker login <安卓镜像仓库地址>”登录。
5. 如果脚本提示“Init:xxxx”相关失败或错误信息，则登录到虚拟机所在节点，通过命令“journalctl -u redroid-android-env -f”查看服务运的相关
   错误日志，如发现是“redroid”进程(仅对于安卓7)起不来请查看虚拟机redroid日志文件：
   /opt/redroid/android-env/env.log/<虚拟机名称>/redroid.log。
6. 如果脚本提示其他错误信息，可通过以下操作进一步排查：
   1）命令“kubectl describe pod -n redroid-android <虚拟机名称>-0”查看虚拟机容器创建过程中的相关事件日志。
   2）通过“kubectl logs pod -n redroid-android <虚拟机名称>-0”查看虚拟机容器初始化过程中的相关日志。
EOF
echo -e "=======================================================================================================================================\n"
}

function info() {
        echo -e "\033[34m[$(date +%F_%T)] INFO   : $@\033[0m"
}
function warn() {
        echo -e "\033[33m[$(date +%F_%T)] WARN   : $@\033[0m"
}
function error() {
        echo -e "\033[31m[$(date +%F_%T)] ERROR  : $@\033[0m"
}
function fatal() {
        echo -e "\033[31m[$(date +%F_%T)] FATAL  : $@\033[0m"
        exit -1
}
function assert() {
        [[ $1 != 0 ]] && fatal "$2"
        return 0
}
function ok() {
        echo -e "\033[32m[$(date +%F_%T)] SUCCESS: $@\033[0m"
}

function ssh_test() {
        which expect > /dev/null
        if [[ $? != 0 ]]; then
                info "准备安装expect..."
                if [[ $OS_TYPE == ubuntu ]]; then        
                        sudo apt-get install -y expect
                else
                        sudo yum install -y expect 
                fi
                assert $? "expect安装失败"
        fi

        local testCmd="ssh -p $NODE_SSH_PORT root@$1 \"ls\""
        timeout 5s expect -c "
        spawn $testCmd
        expect {
        \"yes/no\" { send \"yes\r\"; exp_continue }
        \"password:\" { exit 1 }
}
catch wait exitcode
exit [lindex \$exitcode 3]"
return $?
}

function run_expect_cmd() {
        expect -c "
        set timeout -1
        spawn $@
        expect {
        \"yes/no\" { send \"yes\r\"; exp_continue }
        \"password:\" { exit 1 }
}
catch wait exitcode
exit [lindex \$exitcode 3]"
return $?
}

function stop_android_env() {
        nodeIp=$1
        vmName=$2
        outMsgFile=/tmp/.$$.$vmName

        run_expect_cmd "ssh -p $NODE_SSH_PORT root@$nodeIp \"$NODE_ANDROID_ENV_SH stop $vmName\"" &> $outMsgFile
        if [[ $? != 0 ]]; then
                cat $outMsgFile; rm -rf $outMsgFile
                return -1
        fi
        rm -rf $outMsgFile
        return 0
}
function clear_android_data() {
        nodeIp=$1	
        vmName=$2
        outMsgFile=/tmp/.$$.$vmName

        run_expect_cmd "ssh -p $NODE_SSH_PORT root@$nodeIp \"$NODE_ANDROID_ENV_SH clear $vmName\"" &> $outMsgFile
        if [[ $? != 0 ]]; then
                cat $outMsgFile; rm -rf $outMsgFile
                return -1
        fi
        rm -rf $outMsgFile
        return 0
}
function delete_android_vm() {
        nodeIp=$1
        vmName=$2
        delDiskFlg=$3
        outMsgFile=/tmp/.$$.$vmName

        run_expect_cmd "ssh -p $NODE_SSH_PORT root@$nodeIp \"$NODE_ANDROID_ENV_SH delete $vmName $delDiskFlg\"" &> $outMsgFile
        if [[ $? != 0 ]]; then
                cat $outMsgFile; rm -rf $outMsgFile
                return -1
        fi
        rm -rf $outMsgFile
        return 0
}


function create_android_template_yaml() {
        vmNode=$1
        vmIdxStr=$2
        vmTemplateFile=$3
        vmName=$vmNode-$vmIdxStr
        vmIdxNum=${vmIdxStr#0}
        nodeIdxStr=${vmNode##*-}
        
#        echo "$nodeIdxStr" | grep '^[0-9]\{1,\}$' > /dev/null
#        [[ $? != 0 ]] && fatal "节点（$vmNode）主机名不符合命名规范，无法在该节点上创建云机"
#        
#        nodeIdxStr=`echo "$nodeIdxStr" | grep -o '[1-9][0-9]\{1,\}$'`
#        [[ $nodeIdxStr == "" ]] && nodeIdxStr=0
#        [[ $nodeIdxStr -gt 255 ]] && fatal "节点（$vmNode）主机编号不能大于255"
#
#        nodeIdxHexStr=`printf %02X $nodeIdxStr`

        vmMacAddr=$MAC_COMPANY-$MAC_CLUSTER-00-$vmIdxStr

        vncPort=$(($VNC_BASE_PORT+$vmIdxNum-1))

        adbPort=$(($ADB_BASE_PORT+$vmIdxNum-1))
        p=`cat <<EOF
        - name: adb
          containerPort: 5555
          hostPort: $adbPort
          protocol: TCP	
EOF`
        adbPortCfg=`echo -e "\n$p"`

        rtspLoginPort=$(($RTSP_HOST_BASE_PORT+($vmIdxNum-1)*$RTSP_PORT_NUM))
        rtspPortsCfg=""
        rtspUdpPorts=""

        p=`cat <<EOF
        - name: rtsp-tcp-0
          containerPort: $RTSP_CONTAINER_BASE_PORT
          hostPort: $rtspLoginPort
          protocol: TCP	
EOF`
        rtspPortsCfg=`echo -e "$rtspPortsCfg\n$p"`

        for ((i=0; i<$RTSP_PORT_NUM; i++)); do
                cPort=$(($RTSP_CONTAINER_BASE_PORT+$i))
                hPort=$(($RTSP_HOST_BASE_PORT+$i+($vmIdxNum-1)*$RTSP_PORT_NUM))
                rtspUdpPorts="$rtspUdpPorts,$hPort"
                p=`cat <<EOF
        - name: rtsp-udp-$i
          containerPort: $cPort
          hostPort: $hPort
          protocol: UDP	
EOF`
                rtspPortsCfg=`echo -e "$rtspPortsCfg\n$p"`
        done

	huge2MiPageCfg=""
	[[ $HUGE_PAGE_2MB != "" ]] && huge2MiPageCfg="hugepages-2Mi: ${HUGE_PAGE_2MB}Mi"
		
        sysCpuDir=/sys/devices/system/cpu
        [[ $CPU_BIND_ENABLE == 1 ]] && sysCpuDir=/opt/redroid/android-env/docker/data/$vmName/cpu
		
if [[ $ANDROID_VERSION == 7 ]]; then
cat > $vmTemplateFile <<EOF
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: $vmName
  namespace: redroid-android
  labels:
    nodeName: $vmNode
spec:
  replicas: 1
  serviceName: redroid-android-$vmName
  selector:
    matchLabels:
      vmName: $vmName
  template:
    metadata:
      labels:
        nodeName: $vmNode  
        vmName: $vmName   
        osType: android
      name: $vmName
      namespace: redroid-android
      annotations:
        container.apparmor.security.beta.kubernetes.io/android: unconfined
        #kubernetes.io/ingress-bandwidth: 100M
        #kubernetes.io/egress-bandwidth: 100M
        #cni.projectcalico.org/ipAddrs: "[\"10.244.0.$vmIdxNum\"]"
    spec:
      nodeName: $vmNode
      tolerations:
      - key: node-role.kubernetes.io/master
        effect: NoSchedule
      # 添加以下两个污点容忍是为了禁止node离线后pod被重新调度而导致虚拟机重启
      - key: node.kubernetes.io/not-ready
        effect: NoExecute
      - key: node.kubernetes.io/unreachable
        effect: NoExecute
      terminationGracePeriodSeconds: 0
      dnsConfig:
        nameservers: [ "223.5.5.5", "114.114.114.114" ]
      imagePullSecrets:
      - name: $REGISTRY_SECRET
      initContainers:
      - name: init-android
        image: busybox
        imagePullPolicy: IfNotPresent
        command: [ "/redroid/android-cfg-init.sh" ]
        resources:
          limits:
            redroid/aosp7_binder: 1
        volumeMounts:
        - mountPath: /redroid
          name: volume-redroid
        env:
        - name: POD_NAME
          valueFrom:
            fieldRef:
              fieldPath: metadata.name
        - name: ANDROID_NAME
          value: $vmName
        - name: ANDROID_IDX
          value: "$vmIdxNum"
        - name: ANDROID_VERSION
          value: "$ANDROID_VERSION"
        - name: ANDROID_CPU_BIND_ENABLE
          value: "$CPU_BIND_ENABLE" 
        - name: ANDROID_CPU_BIND_MODE
          value: "$CPU_BIND_MODE" 
        - name: CPU_SHARE_ENABLE
          value: "$CPU_SHARE_ENABLE"
        - name: CPU_GROUP_ENABLE
          value: "$CPU_GROUP_ENABLE_FOR_KP920"
        - name: ANDROID_CPUS
          value: "$CONTAINER_CPUS"
        - name: redroid_CPUS
          value: "$redroid_CPUS"  
        - name: ANDROID_CPU_SETS
          value: "$CONTAINER_CPU_SETS"
        - name: redroid_CPU_SETS
          value: "$redroid_CPU_SETS"  
        - name: redroid_LOG_LEVEL
          value: "$redroid_LOG_LEVEL"  
        - name: ANDROID_VNC_PORT
          value: "$vncPort"
        - name: ANDROID_ADB_PORT
          value: "$adbPort"
        - name: ANDROID_LOGIN_METHOD
          value: $ANDROID_LOGIN_METHOD
        - name: ANDROID_LOGIN_PORT
          value: "$rtspLoginPort"
        - name: ANDROID_RTSP_PORTS
          value: "$rtspUdpPorts"
        - name: ANDROID_VNC_ENABLE
          value: "$VNC_ENABLE"
        - name: SCREEN_WIDTH
          value: "$SCREEN_WIDTH"
        - name: SCREEN_HEIGHT
          value: "$SCREEN_HEIGHT" 
        - name: SCREEN_DPI
          value: "$SCREEN_DPI" 
        - name: SCREEN_FPS
          value: "$SCREEN_FPS" 
        - name: DEFAULT_DISK_SIZE
          value: "$DISK_SIZE" 
        - name: GPU_ENABLE
          value: "$GPU_ENABLE"
        - name: GPU_ALLOC_MODE
          value: "$GPU_ALLOC_MODE" 
        - name: GPU_ID
          value: "$GPU_ID"  
        - name: ENC_ENABLE
          value: "$ENC_ENABLE" 		  
        - name: ENC_ALLOC_MODE
          value: "$ENC_ALLOC_MODE" 
        - name: ENC_ID
          value: "$ENC_ID" 		  
      containers:
      - image: $ANDROID_7_IMAGE
        imagePullPolicy: IfNotPresent
        resources:
          requests:
            cpu: 1
            memory: 1024Mi
            $huge2MiPageCfg
          limits:
            cpu: $CONTAINER_CPUS
            memory: ${MEMORY}Mi
            $huge2MiPageCfg
            redroid/fuse: 1
            redroid/aosp7_ashmem: 1
            redroid/aosp7_binder: 1
            redroid/encode-card-<ENC_IDX>: 1
            #redroid/cyber: 1
        command: [ "/redroid/android-env-init.sh"]
        name: android
        securityContext:
          seccompProfile: 
            localhostProfile: android.json  
            type: Localhost 
          capabilities:
            add: [ "SYS_ADMIN", "NET_ADMIN", "SYS_MODULE", "SYS_NICE", "SYS_TIME", "SYS_TTY_CONFIG", "NET_BROADCAST", "IPC_LOCK", "SYS_RESOURCE", "SYS_PTRACE" ]
        env:
        - name: ANDROID_NAME
          value: $vmName
        - name: ANDROID_IDX
          value: "$vmIdxNum"
        - name: ANDROID_CPUS
          value: "$CONTAINER_CPUS" 
        - name: ANDROID_CPU_BIND_ENABLE
          value: "$CPU_BIND_ENABLE" 
        - name: ANDROID_MAC_ADDRESS
          value: $vmMacAddr
        - name: GPU_ENABLE
          value: "$GPU_ENABLE"
        - name: SCREEN_DPI
          value: "$SCREEN_DPI" 
        - name: SCREEN_ROTATE
          value: "$SCREEN_ROTATE"      
        - name: PATH
          value: /system/bin:/system/xbin
        - name: ANDROID_DATA
          value: /data
        - name: ANDROID_ROOT
          value: /system
        readinessProbe:
          initialDelaySeconds: 10
          periodSeconds: 10
          timeoutSeconds: 60
          successThreshold: 1
          failureThreshold: 6
          exec:
            command: [ "sh", "-c", "ps | grep com.android.launcher3" ]
        volumeMounts:
        - mountPath: /redroid
          name: volume-redroid
        - mountPath: /dev/qemu_pipe:rw
          name: volume-pipe
        - mountPath: /dev/input/event0:rw
          name: volume-event0
        - mountPath: /dev/input/event1:rw
          name: volume-event1
        - mountPath: /dev/input/event2:rw
          name: volume-event2
        - mountPath: /dev/streams:rw
          name: volume-streams
        - mountPath: /data:rw
          name: volume-data
        - mountPath: /share:ro
          name: volume-share
        - mountPath: /apk:ro
          name: volume-apk
        - mountPath: /video:rw
          name: volume-video
        - mountPath: /proc/meminfo:rw
          name: volume-meminfo
        - mountPath: /proc/cpuinfo:rw
          name: volume-cpuinfo
        - mountPath: /proc/diskstats:rw
          name: volume-diskstats
        - mountPath: /proc/stat:rw
          name: volume-stat
        - mountPath: /proc/swaps:rw
          name: volume-swaps
        - mountPath: /proc/uptime:rw
          name: volume-uptime
        - mountPath: /sys/devices/system/cpu:rw
          name: volume-cpus
        - mountPath: /var/run/docker.sock:rw
          name: volume-docker
        ports:$adbPortCfg$rtspPortsCfg
      volumes:
      - name: volume-redroid
        hostPath:
          path: /opt/redroid/android-env/docker
      - name: volume-pipe
        hostPath:
          path: /opt/redroid/android-socket/$vmName/sockets/qemu_pipe
      - name: volume-event0
        hostPath:
          path: /opt/redroid/android-socket/$vmName/input/event0
      - name: volume-event1
        hostPath:
          path: /opt/redroid/android-socket/$vmName/input/event1
      - name: volume-event2
        hostPath:
          path: /opt/redroid/android-socket/$vmName/input/event2
      - name: volume-streams
        hostPath:
          path: /opt/redroid/android-socket/$vmName/streams
      - name: volume-data
        hostPath:
          path: /opt/redroid/android-data/$vmName/data
      - name: volume-share
        hostPath:
          path: /opt/redroid/android-share
      - name: volume-apk
        hostPath:
          path: /opt/redroid/android-apk
      - name: volume-video
        hostPath:
          path: /opt/redroid/android-video
      - name: volume-meminfo
        hostPath:
          path: /var/lib/lxcfs/proc/meminfo
      - name: volume-cpuinfo
        hostPath:
          path: /var/lib/lxcfs/proc/cpuinfo
      - name: volume-diskstats
        hostPath:
          path: /var/lib/lxcfs/proc/diskstats
      - name: volume-stat
        hostPath:
          path: /var/lib/lxcfs/proc/stat
      - name: volume-swaps
        hostPath:
          path: /var/lib/lxcfs/proc/swaps
      - name: volume-uptime
        hostPath:
          path: /var/lib/lxcfs/proc/uptime
      - name: volume-cpus
        hostPath:
          path: $sysCpuDir
      - name: volume-docker
        hostPath:
          path: /var/run/docker.sock
EOF

        if [[ $ENC_IDX == "" ]]; then
                sed -i "/<ENC_IDX>/d" $vmTemplateFile
        else
                sed -i "s/<ENC_IDX>/$ENC_IDX/g" $vmTemplateFile
        fi
elif [[ $ANDROID_VERSION == 9 ]]; then
cat > $vmTemplateFile <<EOF
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: $vmName
  namespace: redroid-android
  labels:
    nodeName: $vmNode
spec:
  replicas: 1
  serviceName: redroid-android-$vmName
  selector:
    matchLabels:
      vmName: $vmName
  template:
    metadata:
      labels:
        nodeName: $vmNode  
        vmName: $vmName   
        osType: android
      name: $vmName
      namespace: redroid-android
      annotations:
        #kubernetes.io/ingress-bandwidth: 100M
        #kubernetes.io/egress-bandwidth: 100M
        #cni.projectcalico.org/ipAddrs: "[\"10.244.0.$vmIdxNum\"]"
    spec:
      nodeName: $vmNode
      tolerations:
      - key: node-role.kubernetes.io/master
        effect: NoSchedule
      # 添加以下两个污点容忍是为了禁止node离线后pod被重新调度而导致虚拟机重启
      - key: node.kubernetes.io/not-ready
        effect: NoExecute
      - key: node.kubernetes.io/unreachable
        effect: NoExecute
      terminationGracePeriodSeconds: 0
      dnsConfig:
        nameservers: [ "223.5.5.5", "114.114.114.114" ]
      imagePullSecrets:
      - name: $REGISTRY_SECRET
      initContainers:
      - name: init-android
        image: busybox
        imagePullPolicy: IfNotPresent
        command: [ "/redroid/android-cfg-init.sh" ]
        volumeMounts:
        - mountPath: /redroid
          name: volume-redroid
        env:
        - name: POD_NAME
          valueFrom:
            fieldRef:
              fieldPath: metadata.name
        - name: ANDROID_NAME
          value: $vmName
        - name: ANDROID_IDX
          value: "$vmIdxNum"
        - name: ANDROID_VERSION
          value: "$ANDROID_VERSION"
        - name: ANDROID_CPU_BIND_ENABLE
          value: "$CPU_BIND_ENABLE" 
        - name: ANDROID_CPU_BIND_MODE
          value: "$CPU_BIND_MODE" 
        - name: CPU_SHARE_ENABLE
          value: "$CPU_SHARE_ENABLE"
        - name: CPU_GROUP_ENABLE
          value: "$CPU_GROUP_ENABLE_FOR_KP920"
        - name: ANDROID_CPUS
          value: "$CONTAINER_CPUS"
        - name: redroid_CPUS
          value: "$redroid_CPUS"  
        - name: ANDROID_CPU_SETS
          value: "$CONTAINER_CPU_SETS"
        - name: redroid_CPU_SETS
          value: "$redroid_CPU_SETS"  
        - name: redroid_LOG_LEVEL
          value: "$redroid_LOG_LEVEL"  
        - name: ANDROID_VNC_PORT
          value: "$vncPort"
        - name: ANDROID_ADB_PORT
          value: "$adbPort"
        - name: ANDROID_LOGIN_METHOD
          value: $ANDROID_LOGIN_METHOD
        - name: ANDROID_LOGIN_PORT
          value: "$rtspLoginPort"
        - name: ANDROID_RTSP_PORTS
          value: "$rtspUdpPorts"
        - name: ANDROID_VNC_ENABLE
          value: "$VNC_ENABLE"
        - name: SCREEN_WIDTH
          value: "$SCREEN_WIDTH"
        - name: SCREEN_HEIGHT
          value: "$SCREEN_HEIGHT" 
        - name: SCREEN_DPI
          value: "$SCREEN_DPI" 
        - name: SCREEN_FPS
          value: "$SCREEN_FPS" 
        - name: DEFAULT_DISK_SIZE
          value: "$DISK_SIZE" 
        - name: GPU_ENABLE
          value: "$GPU_ENABLE"
        - name: GPU_ALLOC_MODE
          value: "$GPU_ALLOC_MODE" 
        - name: GPU_ID
          value: "$GPU_ID" 
        - name: ENC_ENABLE
          value: "$ENC_ENABLE" 		  
        - name: ENC_ALLOC_MODE
          value: "$ENC_ALLOC_MODE" 
        - name: ENC_ID
          value: "$ENC_ID"
      containers:
      - image: "$ANDROID_9_IMAGE"
        imagePullPolicy: IfNotPresent
        resources:
          requests:
            cpu: 1
            memory: 1024Mi
            $huge2MiPageCfg
          limits:
            cpu: $CONTAINER_CPUS
            memory: ${MEMORY}Mi
            $huge2MiPageCfg
        command: [ "/redroid/android-env-init.sh"]
        name: android
        securityContext:
          privileged: true  
        env:
        - name: ANDROID_NAME
          value: $vmName
        - name: ANDROID_IDX
          value: "$vmIdxNum"
        - name: ANDROID_CPUS
          value: "$CONTAINER_CPUS" 
        - name: ANDROID_CPU_BIND_ENABLE
          value: "$CPU_BIND_ENABLE" 
        - name: ANDROID_MAC_ADDRESS
          value: $vmMacAddr
        - name: GPU_ENABLE
          value: "$GPU_ENABLE"
        - name: SCREEN_WIDTH
          value: "$SCREEN_WIDTH"
        - name: SCREEN_HEIGHT
          value: "$SCREEN_HEIGHT" 
        - name: SCREEN_FPS
          value: "$SCREEN_FPS" 
        - name: SCREEN_DPI
          value: "$SCREEN_DPI" 
        - name: SCREEN_ROTATE
          value: "$SCREEN_ROTATE"  
        - name: PATH
          value: /system/bin:/system/xbin
        - name: ANDROID_DATA
          value: /data
        - name: ANDROID_ROOT
          value: /system
        readinessProbe:
          initialDelaySeconds: 10
          periodSeconds: 10
          timeoutSeconds: 60
          successThreshold: 1
          failureThreshold: 6
          exec:
            command: [ "sh", "-c", "ps | grep com.android.launcher3" ]
        volumeMounts:
        - mountPath: /redroid
          name: volume-redroid
        - mountPath: /dev/streams:rw
          name: volume-streams
        - mountPath: /sockets/local:rw
          name: volume-socket-local
        - mountPath: /sockets/share:rw
          name: volume-socket-share
        - mountPath: /data:rw
          name: volume-data
        - mountPath: /share:ro
          name: volume-share
        - mountPath: /apk:ro
          name: volume-apk
        - mountPath: /video:rw
          name: volume-video
#        - mountPath: /proc/meminfo:rw
#          name: volume-meminfo
#        - mountPath: /proc/cpuinfo:rw
#          name: volume-cpuinfo
#        - mountPath: /proc/diskstats:rw
#          name: volume-diskstats
#        - mountPath: /proc/stat:rw
#          name: volume-stat
#        - mountPath: /proc/swaps:rw
#          name: volume-swaps
#        - mountPath: /proc/uptime:rw
#          name: volume-uptime
        - mountPath: /sys/devices/system/cpu:rw
          name: volume-cpus
        - mountPath: /var/run/docker.sock:rw
          name: volume-docker
        ports:$adbPortCfg$rtspPortsCfg
      volumes:
      - name: volume-redroid
        hostPath:
          path: /opt/redroid/android-env/docker
      - name: volume-streams
        hostPath:
          path: /opt/redroid/android-socket/$vmName/streams
      - name: volume-socket-local
        hostPath:
          path: /opt/redroid/android-socket/$vmName
      - name: volume-socket-share
        hostPath:
          path: /opt/redroid/android-socket/share
      - name: volume-data
        hostPath:
          path: /opt/redroid/android-data/$vmName/data
      - name: volume-share
        hostPath:
          path: /opt/redroid/android-share
      - name: volume-apk
        hostPath:
          path: /opt/redroid/android-apk
      - name: volume-video
        hostPath:
          path: /opt/redroid/android-video
#      - name: volume-meminfo
#        hostPath:
#          path: /var/lib/lxcfs/proc/meminfo
#      - name: volume-cpuinfo
#        hostPath:
#          path: /var/lib/lxcfs/proc/cpuinfo
#      - name: volume-diskstats
#        hostPath:
#          path: /var/lib/lxcfs/proc/diskstats
#      - name: volume-stat
#        hostPath:
#          path: /var/lib/lxcfs/proc/stat
#      - name: volume-swaps
#        hostPath:
#          path: /var/lib/lxcfs/proc/swaps
#      - name: volume-uptime
#        hostPath:
#          path: /var/lib/lxcfs/proc/uptime
      - name: volume-cpus
        hostPath:
          path: $sysCpuDir
      - name: volume-docker
        hostPath:
          path: /var/run/docker.sock
EOF
elif [[ $ANDROID_VERSION == 11 || $ANDROID_VERSION == 12 ]]; then
        [[ $ANDROID_VERSION == 11 ]] && vmImage=$ANDROID_11_IMAGE
        [[ $ANDROID_VERSION == 12 ]] && vmImage=$ANDROID_12_IMAGE
        [[ $SCREEN_ROTATE == 0 ]] && vmLandscape=false
        [[ $SCREEN_ROTATE == 1 ]] && vmLandscape=true
        vmMac=${vmMacAddr//-/:}
        
cat > $vmTemplateFile <<EOF
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: $vmName
  namespace: redroid-android
  labels:
    nodeName: $vmNode
spec:
  replicas: 1
  serviceName: redroid-android-$vmName
  selector:
    matchLabels:
      vmName: $vmName
  template:
    metadata:
      labels:
        nodeName: $vmNode  
        vmName: $vmName   
        osType: android
      name: $vmName
      namespace: redroid-android
      annotations:
        #kubernetes.io/ingress-bandwidth: 100M
        #kubernetes.io/egress-bandwidth: 100M
        #cni.projectcalico.org/ipAddrs: "[\"10.244.0.$vmIdxNum\"]"
    spec:
      nodeName: $vmNode
      tolerations:
      - key: node-role.kubernetes.io/master
        effect: NoSchedule
      # 添加以下两个污点容忍是为了禁止node离线后pod被重新调度而导致虚拟机重启
      - key: node.kubernetes.io/not-ready
        effect: NoExecute
      - key: node.kubernetes.io/unreachable
        effect: NoExecute
      terminationGracePeriodSeconds: 0
      dnsConfig:
        nameservers: [ "223.5.5.5", "114.114.114.114" ]
      imagePullSecrets:
      - name: $REGISTRY_SECRET
      initContainers:
      - name: init-android
        image: busybox
        imagePullPolicy: IfNotPresent
        command: [ "/redroid/android-cfg-init.sh" ]
        volumeMounts:
        - mountPath: /redroid
          name: volume-redroid
        env:
        - name: POD_NAME
          valueFrom:
            fieldRef:
              fieldPath: metadata.name
        - name: ANDROID_NAME
          value: $vmName
        - name: ANDROID_IDX
          value: "$vmIdxNum"
        - name: ANDROID_VERSION
          value: "$ANDROID_VERSION"
        - name: ANDROID_CPU_BIND_ENABLE
          value: "$CPU_BIND_ENABLE" 
        - name: ANDROID_CPU_BIND_MODE
          value: "$CPU_BIND_MODE" 
        - name: CPU_SHARE_ENABLE
          value: "$CPU_SHARE_ENABLE"
        - name: CPU_GROUP_ENABLE
          value: "$CPU_GROUP_ENABLE_FOR_KP920"
        - name: ANDROID_CPUS
          value: "$CONTAINER_CPUS"
        - name: redroid_CPUS
          value: "$redroid_CPUS"  
        - name: ANDROID_CPU_SETS
          value: "$CONTAINER_CPU_SETS"
        - name: redroid_CPU_SETS
          value: "$redroid_CPU_SETS"  
        - name: redroid_LOG_LEVEL
          value: "$redroid_LOG_LEVEL"  
        - name: ANDROID_VNC_PORT
          value: "$vncPort"
        - name: ANDROID_ADB_PORT
          value: "$adbPort"
        - name: ANDROID_LOGIN_METHOD
          value: $ANDROID_LOGIN_METHOD
        - name: ANDROID_LOGIN_PORT
          value: "$rtspLoginPort"
        - name: ANDROID_RTSP_PORTS
          value: "$rtspUdpPorts"
        - name: ANDROID_VNC_ENABLE
          value: "$VNC_ENABLE"
        - name: SCREEN_WIDTH
          value: "$SCREEN_WIDTH"
        - name: SCREEN_HEIGHT
          value: "$SCREEN_HEIGHT" 
        - name: SCREEN_DPI
          value: "$SCREEN_DPI" 
        - name: SCREEN_FPS
          value: "$SCREEN_FPS" 
        - name: DEFAULT_DISK_SIZE
          value: "$DISK_SIZE" 
        - name: GPU_ENABLE
          value: "$GPU_ENABLE"
        - name: GPU_ALLOC_MODE
          value: "$GPU_ALLOC_MODE" 
        - name: GPU_ID
          value: "$GPU_ID" 
        - name: ENC_ENABLE
          value: "$ENC_ENABLE" 		  
        - name: ENC_ALLOC_MODE
          value: "$ENC_ALLOC_MODE" 
        - name: ENC_ID
          value: "$ENC_ID"
      containers:
      - image: "$vmImage"
        imagePullPolicy: IfNotPresent
        resources:
          requests:
            cpu: 1
            memory: 1024Mi
            $huge2MiPageCfg
          limits:
            cpu: $CONTAINER_CPUS
            memory: ${MEMORY}Mi
            $huge2MiPageCfg
        command: 
        - "/init"
        - "qemu=1"        
        - "androidboot.hardware=redroid"
        - "net.eth0.dns1=223.5.5.5"
        - "net.eth0.dns2=8.8.8.8"
        - "redroid.mac=$vmMac"
        - "redroid.width=$SCREEN_WIDTH"
        - "redroid.height=$SCREEN_HEIGHT"
        - "redroid.fps=$SCREEN_FPS"
        - "redroid.screen.landscape=$vmLandscape"
        - "ro.sf.lcd_density=$SCREEN_DPI"
        name: android
        securityContext:
          privileged: true  
        env:
        - name: ANDROID_NAME
          value: $vmName
        - name: ANDROID_IDX
          value: "$vmIdxNum"
        - name: ANDROID_CPUS
          value: "$CONTAINER_CPUS" 
        - name: ANDROID_CPU_BIND_ENABLE
          value: "$CPU_BIND_ENABLE" 
        - name: ANDROID_MAC_ADDRESS
          value: $vmMacAddr
        - name: GPU_ENABLE
          value: "$GPU_ENABLE"
        - name: SCREEN_WIDTH
          value: "$SCREEN_WIDTH"
        - name: SCREEN_HEIGHT
          value: "$SCREEN_HEIGHT" 
        - name: SCREEN_FPS
          value: "$SCREEN_FPS" 
        - name: SCREEN_DPI
          value: "$SCREEN_DPI" 
        - name: SCREEN_ROTATE
          value: "$SCREEN_ROTATE"  
        - name: PATH
          value: /system/bin:/system/xbin
        - name: ANDROID_DATA
          value: /data
        - name: ANDROID_ROOT
          value: /system
        readinessProbe:
          initialDelaySeconds: 10
          periodSeconds: 10
          timeoutSeconds: 60
          successThreshold: 1
          failureThreshold: 6
          exec:
            command: [ "sh", "-c", "ps | grep com.android.launcher3" ]
        volumeMounts:
        - mountPath: /redroid
          name: volume-redroid
        - mountPath: /dev/streams:rw
          name: volume-streams
        - mountPath: /data:rw
          name: volume-data
        - mountPath: /share:ro
          name: volume-share
        - mountPath: /apk:ro
          name: volume-apk
        - mountPath: /video:rw
          name: volume-video
#        - mountPath: /proc/meminfo:rw
#          name: volume-meminfo
#        - mountPath: /proc/cpuinfo:rw
#          name: volume-cpuinfo
#        - mountPath: /proc/diskstats:rw
#          name: volume-diskstats
#        - mountPath: /proc/stat:rw
#          name: volume-stat
#        - mountPath: /proc/swaps:rw
#          name: volume-swaps
#        - mountPath: /proc/uptime:rw
#          name: volume-uptime
        - mountPath: /sys/devices/system/cpu:rw
          name: volume-cpus
        - mountPath: /var/run/docker.sock:rw
          name: volume-docker
        ports:$adbPortCfg$rtspPortsCfg
      volumes:
      - name: volume-redroid
        hostPath:
          path: /opt/redroid/android-env/docker
      - name: volume-streams
        hostPath:
          path: /opt/redroid/android-socket/$vmName/streams
      - name: volume-data
        hostPath:
          path: /opt/redroid/android-data/$vmName/data
      - name: volume-share
        hostPath:
          path: /opt/redroid/android-share
      - name: volume-apk
        hostPath:
          path: /opt/redroid/android-apk
      - name: volume-video
        hostPath:
          path: /opt/redroid/android-video
      - name: volume-meminfo
        hostPath:
          path: /var/lib/lxcfs/proc/meminfo
      - name: volume-cpuinfo
        hostPath:
          path: /var/lib/lxcfs/proc/cpuinfo
      - name: volume-diskstats
        hostPath:
          path: /var/lib/lxcfs/proc/diskstats
      - name: volume-stat
        hostPath:
          path: /var/lib/lxcfs/proc/stat
      - name: volume-swaps
        hostPath:
          path: /var/lib/lxcfs/proc/swaps
      - name: volume-uptime
        hostPath:
          path: /var/lib/lxcfs/proc/uptime
      - name: volume-cpus
        hostPath:
          path: $sysCpuDir
      - name: volume-docker
        hostPath:
          path: /var/run/docker.sock
EOF
fi
}


function node_create_android_vms() {
        vmNode=$1
        vmIdxs=$2
        vmsListFile=$VM_MANAGE_DIR/$vmNode/list; echo -n "" > $vmsListFile
        vmsStatusFile=$VM_MANAGE_DIR/$vmNode/status
        successCount=0
        failedCount=0

        for idx in $vmIdxs; do
                vmName=$vmNode-$idx
                info "虚拟机 $vmName 创建中..."
                vmTemplateFile=$VM_MANAGE_DIR/$vmNode/$idx.yaml
                create_android_template_yaml $vmNode $idx $vmTemplateFile 
                kubectl apply -f $vmTemplateFile > /dev/null
                if [[ $? != 0 ]]; then
                        let failedCount++
                        error "虚拟机模板文件 $vmTemplateFile 解析失败"
                else
                        echo "$vmName" >> $vmsListFile
                fi
        done
        [[ `cat $vmsListFile | wc -l` -eq 0 ]] && return

        while true; do
                kubectl get pod -n redroid-android 2> /dev/null | grep $vmNode | awk '{print $1" "$3}' > $vmsStatusFile
                for vm in `cat $vmsListFile`; do
                        vmStatus=`grep $vm $vmsStatusFile`
                        if [[ $? == 0 ]]; then
                                podStatus=${vmStatus##* }
                                podStatusL=`echo $podStatus | tr 'A-Z' 'a-z'`
                                if [[ $podStatusL =~ running ]]; then
                                        let successCount++
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        ok "虚拟机 $vm 创建成功"
                                elif [[ $podStatusL =~ err|fail|backoff|crash ]]; then
                                        let failedCount++
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        error "虚拟机 $vm 创建失败，状态：$podStatus"
                                fi			
                        fi
                done

                if [[ `cat $vmsListFile | wc -l` -eq 0 ]]; then
                        msg="服务器 $vmNode 成功创建虚拟机数目：$successCount，失败数目：$failedCount"
                        [[ $failedCount -ne 0 ]] && error $msg
                        [[ $failedCount -eq 0 ]] && ok $msg
                        echo ""
                        break
                fi
                sleep 0.5
        done
}

function node_startup_android_vms() {
        vmNode=$1
        vmIdxs=$2
        vmsListFile=$VM_MANAGE_DIR/$vmNode/list; echo -n "" > $vmsListFile
        vmsStatusFile=$VM_MANAGE_DIR/$vmNode/status
        successCount=0
        failedCount=0

        for idx in $vmIdxs; do
                vmName=$vmNode-$idx
                cat $ALL_VM_INFO_FILE | grep $vmName | grep /1 > /dev/null
                if [[ $? == 0 ]]; then
                        warn "虚拟机 $vmName 已启动"
                        continue
                fi

                info "虚拟机 $vmName 启动中..."
                kubectl patch statefulset -n redroid-android $vmName -p "{\"spec\":{\"replicas\":1}}" > /dev/null
                if [[ $? != 0 ]]; then
                        let failedCount++
                        error "启动虚拟机 $vmName 出错"
                        continue
                else		
                        echo "$vmName" >> $vmsListFile
                fi
        done
        [[ `cat $vmsListFile | wc -l` -eq 0 ]] && return

        while true; do
                kubectl get pod -n redroid-android 2> /dev/null | grep $vmNode | awk '{print $1" "$3}' > $vmsStatusFile
                for vm in `cat $vmsListFile`; do
                        vmStatus=`grep $vm $vmsStatusFile`
                        if [[ $? == 0 ]]; then
                                podStatus=${vmStatus##* }
                                podStatusL=`echo $podStatus | tr 'A-Z' 'a-z'`
                                if [[ $podStatusL =~ running ]]; then
                                        let successCount++
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        ok "虚拟机 $vm 启动成功"
                                elif [[ $podStatusL =~ err|fail|backoff|crash ]]; then
                                        let failedCount++
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        error "虚拟机 $vm 启动失败，状态：$podStatus"
                                fi			
                        fi
                done

                if [[ `cat $vmsListFile | wc -l` -eq 0 ]]; then
                        msg="服务器 $vmNode 成功启动虚拟机数目：$successCount，失败数目：$failedCount"
                        [[ $failedCount -ne 0 ]] && error $msg
                        [[ $failedCount -eq 0 ]] && ok $msg
                        echo ""
                        break
                fi
                sleep 0.5
        done
}

function node_shutdown_android_vms() {
        vmNode=$1
        vmIdxs=$2
        nodeIp=`grep -w "$vmNode " $ALL_NODE_INFO_FILE | awk '{print $6}'`
        vmsListFile=$VM_MANAGE_DIR/$vmNode/list; echo -n "" > $vmsListFile
        vmsStatusFile=$VM_MANAGE_DIR/$vmNode/status
        successCount=0
        failedCount=0

        # 测试节点ssh服务的免密登录是否开通
        ssh_test $nodeIp > /dev/null
        if [[ $? != 0 ]]; then
                error "服务器 $vmNode($nodeIp) ssh免密登录失败，该服务器上的所有虚拟机关闭任务终止" 
                return
        fi

        for idx in $vmIdxs; do
                vmName=$vmNode-$idx
                cat $ALL_VM_INFO_FILE | grep $vmName | grep /0 > /dev/null
                if [[ $? == 0 ]]; then
                        warn "虚拟机 $vmName 已关闭"
                        continue
                fi

                info "虚拟机 $vmName 关闭中..."
                kubectl patch statefulset -n redroid-android $vmName -p "{\"spec\":{\"replicas\":0}}" > /dev/null
                if [[ $? != 0 ]]; then
                        let failedCount++
                        error "关闭虚拟机 $vmName 出错"
                        continue
                else	
                        stop_android_env $nodeIp $vmName
                        if [[ $? != 0 ]]; then
                                let failedCount++
                                error "虚拟机 $vm 运行环境停止失败"
                                continue
                        fi
                        echo "$vmName" >> $vmsListFile
                fi
        done
        [[ `cat $vmsListFile | wc -l` -eq 0 ]] && return

        while true; do
                kubectl get pod -n redroid-android 2> /dev/null | grep $vmNode | awk '{print $1":"$3}' > $vmsStatusFile
                for vm in `cat $vmsListFile`; do
                        grep $vm $vmsStatusFile > /dev/null 
                        if [[ $? != 0 ]]; then
                                let successCount++
                                sed -i "/^$vm.*/d" $vmsListFile
                                ok "虚拟机 $vm 关闭成功"			
                        fi
                done

                if [[ `cat $vmsListFile | wc -l` -eq 0 ]]; then
                        msg="服务器 $vmNode 成功关闭虚拟机数目：$successCount，失败数目：$failedCount"
                        [[ $failedCount -ne 0 ]] && error $msg
                        [[ $failedCount -eq 0 ]] && ok $msg
                        echo ""
                        break
                fi

                sleep 0.5
        done
}

function node_reboot_upgrade_reset_android_vms() {
        vmNode=$1
        vmIdxs=$2
        nodeIp=`grep -w "$vmNode " $ALL_NODE_INFO_FILE | awk '{print $6}'`
        vmsStatusFile=$VM_MANAGE_DIR/$vmNode/status
        vmsListFile=$VM_MANAGE_DIR/$vmNode/list; echo -n "" > $vmsListFile
        successCount=0
        failedCount=0

        # 测试节点ssh服务的免密登录是否开通
        ssh_test $nodeIp > /dev/null
        if [[ $? != 0 ]]; then
                error "服务器 $vmNode($nodeIp) ssh免密登录失败，该服务器上的所有虚拟机$CMD_CN任务终止" 
                return
        fi

        for idx in $vmIdxs; do
                vmName=$vmNode-$idx
                info "虚拟机 $vmName $CMD_CN中..."
                kubectl patch statefulset -n redroid-android $vmName -p "{\"spec\":{\"replicas\":0}}" > /dev/null
                if [[ $? != 0 ]]; then
                        let failedCount++
                        error "关闭虚拟机 $vmName 出错"
                        continue
                else	
                        stop_android_env $nodeIp $vmName
                        if [[ $? != 0 ]]; then
                                let failedCount++
                                error "虚拟机 $vmName 运行环境停止失败"
                                continue
                        fi

                        if [[ $CMD == "upgrade" ]]; then
                                kubectl patch statefulset -n redroid-android $vmName -p \
                                "{\"spec\":{\"template\":{\"spec\":{\"containers\":[{\"name\":\"android\",\"image\":\"$VM_IMAGE\"}]}}}}" > /dev/null
                                if [[ $? != 0 ]]; then
                                        let failedCount++
                                        error "虚拟机 $vmName 镜像更新失败"
                                        continue
                                fi	
                        fi
                        echo "$vmName" >> $vmsListFile
                fi
        done
        [[ `cat $vmsListFile | wc -l` -eq 0 ]] && return

        tmpVmsListFile=$vmsListFile.tmp; echo -n "" > $tmpVmsListFile
        cp $vmsListFile $tmpVmsListFile

        while true; do
                kubectl get pod -n redroid-android 2> /dev/null | grep $vmNode | awk '{print $1" "$3}' > $vmsStatusFile
                for vm in `cat $vmsListFile`; do
                        vmStatus=`grep $vm $vmsStatusFile`
                        if [[ $? != 0 ]]; then
                                if [[ $CMD == "reset" ]]; then
                                        clear_android_data $nodeIp $vm
                                        if [[ $? != 0 ]]; then
                                                let failedCount++
                                                sed -i "/^$vm.*/d" $vmsListFile
                                                error "虚拟机 $vm 数据删除失败"
                                                continue
                                        fi
                                fi

                                kubectl patch statefulset -n redroid-android $vm -p "{\"spec\":{\"replicas\":1}}" > /dev/null
                                if [[ $? != 0 ]]; then
                                        let failedCount++
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        error "启动虚拟机 $vm 出错"
                                        continue
                                fi

                                echo $vm >> $tmpVmsListFile
                        else
                                grep $vm $tmpVmsListFile > /dev/null
                                [[ $? != 0 ]] && continue

                                podStatus=${vmStatus##* }
                                podStatusL=`echo $podStatus | tr 'A-Z' 'a-z'`
                                if [[ $podStatusL =~ running ]]; then
                                        let successCount++
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        ok "虚拟机 $vm $CMD_CN成功"
                                elif [[ $podStatusL =~ err|fail|backoff|crash ]]; then
                                        let failedCount++
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        error "虚拟机 $vm $CMD_CN失败，状态：$podStatus"
                                        continue
                                fi
                        fi
                done

                if [[ `cat $vmsListFile | wc -l` -eq 0 ]]; then
                        msg="服务器 $vmNode 成功$CMD_CN虚拟机数目：$successCount，失败数目：$failedCount"
                        [[ $failedCount -ne 0 ]] && error $msg
                        [[ $failedCount -eq 0 ]] && ok $msg
                        echo ""
                        break
                fi
                sleep 0.5
        done
}
function node_reboot_android_vms() {
        node_reboot_upgrade_reset_android_vms "$1" "$2"
}
function node_upgrade_android_vms() {
        node_reboot_upgrade_reset_android_vms "$1" "$2"
}
function node_reset_android_vms() {
        node_reboot_upgrade_reset_android_vms "$1" "$2"
}

function node_delete_android_vms() {
        vmNode=$1
        vmIdxs=$2
        nodeIp=`grep -w "$vmNode " $ALL_NODE_INFO_FILE | awk '{print $6}'`
        vmsListFile=$VM_MANAGE_DIR/$vmNode/list; echo -n "" > $vmsListFile
        vmsStatusFile=$VM_MANAGE_DIR/$vmNode/status
        successCount=0
        failedCount=0

        # 测试节点ssh服务的免密登录是否开通
        ssh_test $nodeIp > /dev/null
        if [[ $? != 0 ]]; then
                error "服务器 $vmNode($nodeIp) ssh免密登录失败，该服务器上的所有虚拟机删除任务终止" 
                return
        fi

        for idx in $vmIdxs; do
                vmName=$vmNode-$idx
                info "虚拟机 $vmName 删除中..."
                kubectl patch statefulset -n redroid-android $vmName -p "{\"spec\":{\"replicas\":0}}" > /dev/null
                if [[ $? != 0 ]]; then
                        let failedCount++
                        error "关闭虚拟机 $vmName 出错"
                        continue
                else
                        stop_android_env $nodeIp $vmName
                        if [[ $? != 0 ]]; then
                                let failedCount++
                                error "虚拟机 $vm 运行环境停止失败"
                                continue
                        fi
                        echo "$vmName" >> $vmsListFile
                fi
        done
        [[ `cat $vmsListFile | wc -l` -eq 0 ]] && return

        while true; do
                kubectl get pod -n redroid-android 2> /dev/null | grep $vmNode | awk '{print $1" "$3}' > $vmsStatusFile
                for vm in `cat $vmsListFile`; do
                        grep $vm $vmsStatusFile > /dev/null 
                        if [[ $? != 0 ]]; then
                                if [[ $VM_DATA_DELETE_FLG == "y" ]]; then
                                        delete_android_vm $nodeIp $vm 1
                                else
                                        delete_android_vm $nodeIp $vm 0
                                fi

                                if [[ $? != 0 ]]; then
                                        let failedCount++
                                        error "虚拟机 $vm 删除失败"
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        continue
                                fi

                                kubectl delete statefulset -n redroid-android $vm > /dev/null
                                if [[ $? != 0 ]]; then
                                        let failedCount++
                                        error "虚拟机 $vm 删除失败"
                                        sed -i "/^$vm.*/d" $vmsListFile
                                        continue
                                fi								

                                let successCount++
                                sed -i "/^$vm.*/d" $vmsListFile
                                ok "虚拟机 $vm 删除成功"			
                        fi
                done

                if [[ `cat $vmsListFile | wc -l` -eq 0 ]]; then
                        msg="服务器 $vmNode 成功删除虚拟机数目：$successCount，失败数目：$failedCount"
                        [[ $failedCount -ne 0 ]] && error $msg
                        [[ $failedCount -eq 0 ]] && ok $msg
                        echo ""
                        break
                fi

                sleep 0.5
        done
}

function get_android_vms_view() {
        vmsInfoFile=$VM_MANAGE_DIR/vms.info
        kubectl get statefulset -n redroid-android | sed '1d' | awk '{print $1":"$2}' > $vmsInfoFile

        readyCount=0
        noReadyCount=0
        shutdownCount=0
        echo -e "\n服务器上的虚拟机统计信息："
        for node in $NODES; do
                [[ $node == "-" ]] && node=$K8S_NODE_NAME
                echo -e "=============== \033[34m$node\033[0m ==============="
                printf "%-32s%-32s\n" "NAME" "STATUS"
                ndoeVmsInfoFile=$VM_MANAGE_DIR/.vms.info.tmp
                grep $node $vmsInfoFile > $ndoeVmsInfoFile
                nodeReadyCount=0; nodeNoReadyCount=0; nodeShutdownCount=0
                for line in `cat $ndoeVmsInfoFile`; do
                        vmName=${line%%:*}
                        vmState=${line##*:}
                        if [[ $vmState == "0/0" ]]; then
                                let nodeShutdownCount++; let shutdownCount++
                                printf "%-32s\033[33m关机\033[0m\n" $vmName
                        elif [[ $vmState == "0/1" ]]; then
                                let nodeNoReadyCount++; let noReadyCount++
                                printf "%-32s\033[31m开机未就绪\033[0m\n" $vmName
                        elif [[ $vmState == "1/1" ]]; then
                                let nodeReadyCount++; let readyCount++
                                printf "%-32s\033[32m正常\033[0m\n" $vmName
                        fi
                done
                echo -e "\n正常：\033[32m$nodeReadyCount\033[0m，开机未就绪：\033[31m$nodeNoReadyCount\033[0m，关机：\033[33m$nodeShutdownCount\033[0m"
        done
        echo -e "-------------------------------------------"
        echo -e "总正常：\033[32m$readyCount\033[0m，总开机未就绪：\033[31m$noReadyCount\033[0m，总关机：\033[33m$shutdownCount\033[0m"
        echo ""
}

function get_android_vms_status() {
        vmsStatusFile=$VM_MANAGE_DIR/vms.status
        kubectl get pod -n redroid-android > $vmsStatusFile

        readyCount=0
        noReadyCount=0
        errorCount=0
        echo -e "\n服务器上的虚拟机运行状态："
        for node in $NODES; do
                [[ $node == "-" ]] && node=$K8S_NODE_NAME
                echo -e "=========================================== \033[34m$node\033[0m ==========================================="
                printf "%-32s%-32s%-32s%-32s\n" "NAME" "READY" "STATUS" "AGE"
                ndoeVmsStatusFile=$VM_MANAGE_DIR/.vms.status.tmp
                grep $node $vmsStatusFile > $ndoeVmsStatusFile
                nodeReadyCount=0; nodeNoReadyCount=0;nodeErrorCount=0
                while read line; do
                        eval $(echo $line | awk '{printf("name=%s; ready=%s; status=%s; age=%s;",$1,$2,$3,$5)}')
                        if [[ $ready == "0/1" ]]; then
                                statusL=`echo $status | tr 'A-Z' 'a-z'`
                                if [[ $statusL =~ err|fail|backoff|crash ]]; then
                                        let nodeErrorCount++; let errorCount++
                                fi
                                let nodeNoReadyCount++; let noReadyCount++
                                printf "%-32s\033[31m%-32s%-32s\033[0m%-32s\n" ${name%-0} 0 $status $age
                        elif [[ $ready == "1/1" ]]; then
                                let nodeReadyCount++; let readyCount++ 
                                printf "%-32s\033[32m%-32s%-32s\033[0m%-32s\n" ${name%-0} 1 $status $age
                        fi	
                done < $ndoeVmsStatusFile
                echo -e "\n正常：\033[32m$nodeReadyCount\033[0m，未就绪：\033[31m$nodeNoReadyCount\033[0m，异常：\033[31m$nodeErrorCount\033[0m"
        done
        echo "-----------------------------------------------------------------------------------------------------"
        echo -e "总正常：\033[32m$readyCount\033[0m，总未就绪：\033[31m$noReadyCount\033[0m，总异常：\033[31m$errorCount\033[0m"
        echo ""
}

function get_android_vms_inspect() {
        which jq > /dev/null
        if [[ $? != 0 ]]; then
                echo -n "检测到未安装“jq”工具，请输入sudo密码安装进行安装："
                sudo apt-get install -y jq > /dev/null
                [[ $? != 0 ]] && fatal "安装jq失败"
        fi

        vmConfigFile=$VM_MANAGE_DIR/vm.config
        kubectl get pod -n redroid-android $VM-0 -ojson > $vmConfigFile 2> /dev/null
        if [[ $? != 0 ]]; then
                kubectl get statefulset -n redroid-android $VM -ojson > $vmConfigFile 2> /dev/null
                if [[ $? != 0 ]]; then
                        fatal "虚拟机 $VM 不存在"
                fi
                cat $vmConfigFile | jq .spec
        else
                cat $vmConfigFile | jq .
        fi
}

function get_android_vms_config() {
        vmTemplateFile=$VM_MANAGE_DIR/vm.template

        kubectl get statefulset -n redroid-android $VM -oyaml > $vmTemplateFile 
        [[ $? != 0 ]] && fatal "无法获取虚拟机 $VM 的信息" 

        vmNode=`cat $vmTemplateFile | grep "\s\+nodeName:" | head -1 | awk -F ":" '{print $2}'`
        nodeIp=`kubectl get node $vmNode -o wide | grep -w $vmNode | awk '{print $6}'`

        # 测试节点ssh服务的免密登录是否开通
        ssh_test $nodeIp > /dev/null
        [[ $? != 0 ]] && fatal "服务器 $vmNode($nodeIp) ssh免密登录失败，无法获取虚拟机配置信息" 

        run_expect_cmd "ssh -p $NODE_SSH_PORT root@$nodeIp \"cat /opt/redroid/android-env/docker/data/$VM/init_cfg\"" | sed '1d'
}


function process_cmdline_params() {
        [[ $1 != "view" ]] &&
        [[ $1 != "status" ]] &&
        [[ $1 != "inspect" ]] &&
        [[ $1 != "config" ]] &&
        [[ $1 != "create" ]] &&
        [[ $1 != "startup" ]] &&
        [[ $1 != "shutdown" ]] &&
        [[ $1 != "reboot" ]] &&
        [[ $1 != "upgrade" ]] && 
        [[ $1 != "reset" ]] &&
        [[ $1 != "delete" ]] || 
        [[ $1 == "help" ]] || 
        [[ $1 == "-h" ]] || 
        [[ $1 == "--help" ]] && usage
        CMD=$1
        [[ $CMD == "create" ]] && CMD_CN="创建"
        [[ $CMD == "startup" ]] && CMD_CN="启动"
        [[ $CMD == "shutdown" ]] && CMD_CN="关闭"
        [[ $CMD == "reboot" ]] && CMD_CN="重启"
        [[ $CMD == "upgrade" ]] && CMD_CN="升级"
        [[ $CMD == "reset" ]] && CMD_CN="重置"
        [[ $CMD == "delete" ]] && CMD_CN="删除"
        shift

        while [[ $# -gt 0 ]]; do
                case $1 in
                        -nl|--node-list) NODES=$2;;
                        -vl|--vm-list) VMS=$2;;
                        -vn|--vm-name) VM=$2;;
                        -vi|--vm-image)	VM_IMAGE=$2;; 
                        *) fatal "无效的命令选项: $1"
                esac
                shift
                shift
        done

        [[ ($CMD == "view" || $CMD == "status") && $NODES == "" ]] && usage
        [[ ($CMD == "inspect" || $CMD == "config") && $VM == "" ]] && usage
        [[ ($CMD == "create" || $CMD == "startup" || $CMD == "shutdown" || $CMD == "reboot" || $CMD == "delete") && $VMS == "" ]] && usage
        [[ $CMD == "upgrade" && ($VMS == "" || $VM_IMAGE == "") ]] && usage
        [[ $CMD == "create" && $VMS == "all" ]] && fatal "“$CMD”命令所携带的“-vl”选项值不能为“all”"


        if [[ $CMD == "view" || $CMD == "status" ]]; then
                local allNode=`kubectl get node | grep -v -w NAME | awk '{print $1}'`
                if [[ $NODES == "all" ]]; then
                        NODES=$allNode
                else
                        NODES=${NODES//,/ }
                        for node in $NODES; do
                                if [[ $node =~ ^[0-9a-zA-Z_-]+$ ]]; then
                                        [[ $node == "-" ]] && node=$K8S_NODE_NAME
                                        echo "$allNode " | grep "$node " > /dev/null
                                        if [[ $? != 0 ]]; then
                                                fatal "无法获取服务器($node)的信息，请确认该服务器已经加入集群"
                                        fi
                                else
                                        fatal "无效的服务器名：$node"
                                fi
                        done
                fi
                return
        elif [[ $CMD == "inspect" || $CMD == "config" ]]; then
                return
        fi

        kubectl get statefulset -n redroid-android 2> /dev/null > $ALL_VM_INFO_FILE 
        kubectl get node -o wide 2> /dev/null > $ALL_NODE_INFO_FILE 

        if [[ $VMS == "all" ]]; then
                local nodes=`grep -v -w NAME $ALL_NODE_INFO_FILE | awk '{print $1}'`
                for node in $nodes; do
                        local vms=`grep "$node-[0-9][0-9] " $ALL_VM_INFO_FILE | awk '{print $1}'`
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
                                grep -w "$node " $ALL_NODE_INFO_FILE > /dev/null
                                if [[ $? != 0 ]]; then
                                        fatal "无法获取服务器 $node 的信息，请确认该服务器已经加入集群"
                                fi
                        else
                                fatal "无效的服务器名：$node"
                        fi

                        for k in ${!NODES_VMS_MAP[@]}; do
                                [[ $k == $node ]] && fatal "重复的服务器：$node"
                        done

                        local vmIdxs=${nodeVms##*:}
                        local vmIdxSet=
                        vmIdxs=${vmIdxs//,/ }
                        if [[ $vmIdxs != "all" ]]; then
                                for idx in $vmIdxs; do
                                        if [[ $idx =~ ^[1-9][0-9]{0,}$ ]]; then
                                                vmIdxSet="$vmIdxSet `printf "%02d" $idx`"
                                        elif [[ $idx =~ ^[1-9][0-9]{0,}-[1-9][0-9]{0,}$ ]]; then
                                                local startIdx=${idx%%-*}
                                                local endIdx=${idx##*-}
                                                while [[ $startIdx -le $endIdx ]]; do
                                                        vmIdxSet="$vmIdxSet `printf "%02d" $startIdx`"
                                                        let startIdx++
                                                done
                                        else
                                                fatal "无效的虚拟机编号：$VMS"
                                        fi
                                done
                        fi

                        local vmNames=
                        if [[ $vmIdxs == "all" ]]; then
                                if [[ $CMD == "create" ]]; then
                                        fatal "“$CMD”命令所携带的“-vl”选项值中的虚拟机编号列表不能为“all”"
                                else 
                                        vmNames=`grep "$node-[0-9][0-9] " $ALL_VM_INFO_FILE | awk '{print $1}'`
                                fi

                                for n in $vmNames; do
                                        vmIdxSet="$vmIdxSet ${n##*-}"
                                done
                        elif [[ $CMD != "create" ]]; then
                                vmNames=`grep $node $ALL_VM_INFO_FILE | awk '{print $1}'`
                                for idx in $vmIdxSet; do
                                        local flg=0
                                        local vmPod=$node-`printf "%02d" ${idx#0}`
                                        for n in $vmNames; do
                                                [[ $vmPod == $n ]] && flg=1 
                                        done

                                        [[ $flg == 0 ]] && fatal "服务器 $node 上不存在$idx号虚拟机"
                                done
                        elif [[ $CMD == "create" ]]; then
                                vmNames=`grep $node $ALL_VM_INFO_FILE | awk '{print $1}'`
                                for idx in $vmIdxSet; do
                                        local vmPod=$node-`printf "%02d" ${idx#0}`
                                        for n in $vmNames; do
                                                [[ $vmPod == $n ]] && fatal "服务器 $node 上已存在$idx号虚拟机，如需重新创建请先将其删除"
                                        done
                                done
                        fi

                        NODES_VMS_MAP[$node]=$vmIdxSet	
                done
        fi

        # 打印虚拟机MAP映射表 
        #for k in ${!NODES_VMS_MAP[@]}; do
        #	warn "$k:${NODES_VMS_MAP[$k]}"
        #done
        #exit
}

function main() {
        get_os_info     
        
        if [[ ! -e $VM_MANAGE_DIR ]]; then
                mkdir -p $VM_MANAGE_DIR
                [[ $? != 0 ]] && fatal "无法创建文件夹：$VM_MANAGE_DIR"
        fi

        process_cmdline_params "$@"

        if [[ $CMD == "view" ]]; then
                get_android_vms_view
                return
        elif [[ $CMD == "status" ]]; then
                get_android_vms_status
                return
        elif [[ $CMD == "inspect" ]]; then
                get_android_vms_inspect
                return
        elif [[ $CMD == "config" ]]; then
                get_android_vms_config
                return
        elif [[ $CMD == "create" ]]; then
                while true; do
                        [[ $ANDROID_VERSION == "7" || $ANDROID_VERSION == "9" || $ANDROID_VERSION == "11" || $ANDROID_VERSION == "12" ]] && break
                        read -p "请确认创建的安卓虚拟机版本(7/9/11/12): " ANDROID_VERSION
                done

                kubectl get namespace | grep -w redroid-android &> /dev/null
                [[ $? != 0 ]] &&  kubectl create namespace redroid-android > /dev/null

                process_vm_config
        fi

        error_debug_guide

        for node in ${!NODES_VMS_MAP[@]}; do
                nodeVmManageDir=$VM_MANAGE_DIR/$node
                if [[ ! -e $nodeVmManageDir ]]; then
                        mkdir -p $nodeVmManageDir
                        if [[ $? != 0 ]]; then
                                error "无法创建文件夹：$nodeVmManageDir"
                                contiue
                        fi
                fi

                nodeVms=${NODES_VMS_MAP[$node]}
                if [[ $nodeVms != "" ]]; then
                        if [[ $CMD == delete ]]; then
                                while true; do
                                        [[ $VM_DATA_DELETE_FLG == "y" || $VM_DATA_DELETE_FLG == "n" ]] && break
                                        read -p "是否删除虚拟机磁盘数据(y/n): " VM_DATA_DELETE_FLG
                                done
                        fi
                        node_${CMD}_android_vms "$node" "$nodeVms" &
                fi
        done
        wait
}
main "$@"
exit $?

