#!/bin/bash
#set -x

##############################################################################
CLUSTER_NAME=kubernetes
LONGITUDE=113.88308
LATITUDE=22.55329
K8S_VERSION=1.23.17
DOCKER_VERSION=20.10.23
POD_CIDR=10.244.0.0/16
POD_CIDR6=fd00::/96
SERVICE_CIDR=10.96.0.0/12
DATA_FILE=./etcd.db
IMAGE_REGISTRY=registry.cn-hangzhou.aliyuncs.com/google_containers
##############################################################################
CERTS_KEY=128439afa89e6946d962881ea321fe0236a58026551a24211892a0dd1fe7d165
HA_IP=
HA_PORT=
MASTER_IP=
MASTER_PORT=
WORKER_IP=
NODE_TYPE=
JOIN_TOKEN=
CMD=
OS_TYPE=
NODE_IP=
NETWORK_PLUGIN_FILE=
K8S_UPGRADE_FLG=0
DOCKER_UPGRADE_FLG=0
CUR_NODE_TYPE=

function info() {
	echo -e "\033[34m[$(date +%F_%T)] INFO   : $@\033[0m"
}
function warn() {
	echo -e "\033[33m[$(date +%F_%T)] WARN   : $@\033[0m"
}
function error() {
	echo -e "\033[31m[$(date +%F_%T)] ERROR  : $@\033[0m" >&2
}
function fatal() {
	echo -e "\033[31m[$(date +%F_%T)] FATAL  : $@\033[0m" >&2
	exit -1
}
function assert() {
	[[ $1 != 0 ]] && fatal "$2"
	return 0
}
function ok() {
	echo -e "\033[32m[$(date +%F_%T)] SUCCESS: $@\033[0m"
}

function str_lt() { 
	test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" != "$1"; 
}

function get_os_info() {
	cat /etc/issue 2> /dev/null | grep Ubuntu > /dev/null
	if [[ $? == 0 ]]; then
		OS_TYPE=ubuntu
	else
		info=`cat /etc/redhat-release 2> /dev/null`
		if [[ $info != "" ]]; then
			if [[ $info =~ CentOS ]]; then
				OS_TYPE=centos
				which yum-config-manager &> /dev/null
				if [[ $? != 0 ]]; then
					yum install -y yum-utils 
					[[ $? != 0 ]] && fatal "安装yum-utils失败"
				fi
			elif [[ $info =~ BigCloud ]]; then
				OS_TYPE=bclinux
			else
				fatal "不支持的操作系统类型，目前仅支持ubuntu，centos和bclinux"
			fi
		else
			fatal "不支持的操作系统类型，目前仅支持ubuntu，centos和bclinux"
		fi
	fi
}

 function get_node_type() {
        v=`ls /etc/kubernetes/manifests 2> /dev/null`
        if [[ $v != "" ]]; then
                CUR_NODE_TYPE=master
        else    
                v=`ls /etc/kubernetes/pki 2> /dev/null`
                [[ $v != "" ]] && CUR_NODE_TYPE=worker
        fi
 }

function get_node_ip() {
        eth=`ip route list | grep default | head -1 | awk '{print $5}'`
	ip=`ip addr show $eth | grep -w inet | head -1 | awk '{print $2}'`
	NODE_IP=${ip%%/*}
}

function check_ip() {
        nodeIp=$1
        ethDev=`ip addr show | grep -w inet | grep $nodeIp/ | awk '{print $NF}'`
	if [[ $ethDev == "" ]]; then
               fatal "$nodeIp 不存在"
        else
                ip route list | grep via | grep $ethDev > /dev/null
                [[ $? != 0 ]] && fatal "$nodeIp 没有配置网关路由"
        fi
}

function add_host_dns() {
	nodeIp=$1
	dns=`cat /etc/hosts | grep $(hostname)`
	if [[ $dns != "" ]]; then
		echo $dns | grep $nodeIp > /dev/null
		if [[ $? != 0 ]]; then
			sed -i "/$(hostname)/d" /etc/hosts > /dev/null			
			echo "$nodeIp $(hostname)" >> /etc/hosts
		fi
	else
		echo "$nodeIp $(hostname)" >> /etc/hosts
	fi
}

function usage() {
cat <<EOF
Usage: `basename $0` COMMAND [OPTIONS]

Commands:
    init            初始化集群master节点，使用方法如下：
                    init [-kv <K8S版本>] [-cn <集群名称>] [-mi <API服务IP>] [-mp <API服务PORT>] [-hi <API服务高可用IP>] [-hp <API服务高可用PORT>] [-pc <POD IPV4 CIDR>] 
                    [-pc6 <POD IPV6 CIDR>] -npf <集群网络插件安装文件> [-dv <DOCKER版本>]

    reinit          重新初始化集群master节点，使用方法如下：
                    reinit [-kv <K8S版本>] [-cn <集群名称>] [-mi <API服务IP>] [-mp <API服务PORT>] [-hi <API服务高可用IP>] [-hp <API服务高可用PORT>] [-pc <POD IPV4 CIDR>] 
                    [-pc6 <POD IPV6 CIDR>] [-dv <DOCKER版本>]
    
    join            加入worker节点到集群，使用方法如下：
                    join -nt <节点类型> [-wi <WORKER节点IP>] [-mi <API服务IP>] [-mp <API服务PORT>] -hi <API服务高可用IP> [-hp <API服务高可用PORT>] -jt <节点加入TOKEN> [-dv <DOCKER版本>]

    rejoin          重新加入worker节点到集群，使用方法如下：
                    rejoin -nt <节点类型> [-wi <WORKER节点IP>] [-mi <API服务IP>] [-mp <API服务PORT>] -hi <API服务高可用IP> [-hp <API服务高可用PORT>] -jt <节点加入TOKEN> [-dv <DOCKER版本>]

    reset           复位集群k8s运行环境

    upgrade         升级集群k8s运行环境，用方法如下：
                    upgrade [-kv <K8S版本>] [-dv <DCOKER版本>]
    
    status          在集群master节点上查看集群运行状态

    create-token    在集群master节点上生成join token    

    update-certs    更新集群master节点证书 
                
    backup-data     备份集群master的etcd数据到指定目录，使用方法如下：
                    backup-data [-dr <备份数据文件路径>]

    restore-data    从指定文件恢复集群etcd数据，使用方法如下：
                    restore-data -df <备份数据文件路径>


Options:
    -cn, --cluster-name          string      集群名称，默认为“kubernetes”
    --longitude                  float       集群位置经度，默认为“$LONGITUDE”
    --latitude                   float       集群位置纬度，默认为“$LATITUDE”
    -kv, --k8s-version           string      集群k8s版本，默认为“$K8S_VERSION”
    -dv, --docker-version        string      集群docker版本，默认为“$DOCKER_VERSION”
    -hi, --ha-ip                 string      集群api服务高可用ip，默认为“-mi”选项的值
    -hp, --ha-port               int         集群api服务高可用port，默认为“-mp”选项的值
    -mi, --master-ip             string      集群master节点的api服务绑定的本机物理网卡ip（master节点init成功后可上面通过命令kubectl cluster-info查看），
                                             默认为本机流量默认出口ip
    -mp, --master-port           int         集群master节点的api服务绑定的本机port，默认为6443
    -wi, --worker-ip             string      集群worker节点加入集群的本机物理网卡ip，默认为本机流量默认出口ip
    -nt, --node-type             string      集群节点类型，其值只能为“master”或“worker”
    -jt, --join-token            string      集群节点加入token，可在master节点上通过该脚本的create-token命令生成
    -pc, --pod-cidr              string      集群pod容器的ipv4地址范围，默认为“$POD_CIDR”
    -pc6, --pod-cidr6            string      集群pod容器的ipv6地址范围，默认为“$POD_CIDR6”
    -df, --data-file             string      集群etcd备份数据文件路径，默认为“$DATA_FILE”
    -npf, --network-plugin-file  string      集群网络插件安装文件路径

EOF
exit
}

function process_cmdline_params() {
	[[ $1 != "init" ]] &&
	[[ $1 != "reinit" ]] &&
	[[ $1 != "join" ]] &&
	[[ $1 != "rejoin" ]] &&
	[[ $1 != "reset" ]] &&
	[[ $1 != "upgrade" ]] &&
        [[ $1 != "status" ]] &&
        [[ $1 != "create-token" ]] &&
        [[ $1 != "check-certs" ]] &&
        [[ $1 != "update-certs" ]] &&
	[[ $1 != "backup-data" ]] &&
	[[ $1 != "restore-data" ]] || 
	[[ $1 == "help" ]] || 
	[[ $1 == "-h" ]] || 
	[[ $1 == "--help" ]] && usage
	
	CMD=$1
	shift
	while [[ $# -gt 0 ]]; do
		case $1 in
			-kv|--k8s-version)   K8S_VERSION=$2;K8S_UPGRADE_FLG=1;;
			-dv|--docker-version)DOCKER_VERSION=$2;DOCKER_UPGRADE_FLG=1;;
			-cn|--cluster-name)  CLUSTER_NAME=$2;;
                        --longitude)         LONGITUDE=$2;;
                        --latitude)          LATITUDE=$2;;
			-hi|--ha-ip)         HA_IP=$2;;
			-hp|--ha-port)       HA_PORT=$2;;
			-mi|--master-ip)     MASTER_IP=$2;;
			-mp|--master-port)   MASTER_PORT=$2;;
			-nt|--node-type)     NODE_TYPE=$2;;
			-wi|--worker-ip)     WORKER_IP=$2;;
			-jt|--join-token)    JOIN_TOKEN=$2;;
			-pc|--pod-cidr)      POD_CIDR=$2;;
			-pc6|--pod-cidr6)    POD_CIDR6=$2;;
			-df|--data-file)     DATA_FILE=$2;;
                        -npf|--network-plugin-file) NETWORK_PLUGIN_FILE=$2;;
			*) fatal "无效的命令选项: $1"
		esac
		shift
		shift
	done

	if [[ $CMD == "init" || $CMD == "reinit" ]]; then
                if [[ $MASTER_IP == "" ]]; then
                        get_node_ip
                        MASTER_IP=$NODE_IP
                fi
		[[ $MASTER_PORT == "" ]] && MASTER_PORT=6443
		[[ $HA_IP == "" ]] && HA_IP=$MASTER_IP
		[[ $HA_PORT == "" ]] && HA_PORT=$MASTER_PORT

                if [[ $CMD == "init" ]]; then
                       [[ $NETWORK_PLUGIN_FILE == "" ]] && fatal "选项-npf的值为空"
                       [[ ! -e $NETWORK_PLUGIN_FILE ]] && fatal "集群网络插件安装文件（$NETWORK_PLUGIN_FILE）不存在"
                       [[ ! $LONGITUDE =~ ^[-0-9.]{1,}$ ]] && fatal "无效的经度值：$LONGITUDE"
                       [[ ! $LATITUDE =~ ^[-0-9.]{1,}$ ]] && fatal "无效的纬度值：$LATITUDE"
                fi
	elif [[ $CMD == "join" || $CMD == "rejoin" ]]; then
                if [[ $NODE_TYPE == "master" ]]; then
                        if [[ $MASTER_IP == "" ]]; then
                                get_node_ip
                                MASTER_IP=$NODE_IP
                        fi
                        [[ $MASTER_PORT == "" ]] && MASTER_PORT=6443
                elif [[ $NODE_TYPE == "worker" ]]; then
                        if [[ $WORKER_IP == "" ]]; then
                               get_node_ip
                               WORKER_IP=$NODE_IP
                        fi
                else
                        fatal "选项-nt的值必须为“master”或“worker”"
                fi
		[[ $JOIN_TOKEN == "" ]] && fatal "集群节点加入token为空"
		[[ $HA_IP == "" ]] && fatal "集群api服务高可用ip为空"
		[[ $HA_PORT == "" ]] && HA_PORT=6443
        elif [[ $CMD == "upgrade" ]]; then
                [[ $K8S_UPGRADE_FLG == 0 && $DOCKER_UPGRADE_FLG == 0 ]] && fatal "选项-kv和-dv不能同时为空"
		[[ $DOCKER_UPGRADE_FLG == 1 && $DOCKER_VERSION == "" ]] && fatal "选项-dv的值为空"
		[[ $K8S_UPGRADE_FLG == 1 && $K8S_VERSION == "" ]] && fatal "选项-kv的值为空"
                [[ $K8S_UPGRADE_FLG == 1 && $NODE_TYPE != "master" && $NODE_TYPE != "worker" ]] && fatal "选项-nt的值必须为master或worker"
	elif [[ $CMD == "backup-data" || $CMD == "restore-data" ]]; then
		[[ $DATA_FILE == "" ]] && DATA_FILE=./etcd.db
		if [[ $CMD == "restore-data" ]]; then
			[[ ! -e $DATA_FILE ]] && fatal "etcd备份数据文件（$DATA_FILE）不存在"
		fi
	fi
}

KUBEADM_INIT_CONFIG_FILE="kubeadm-init-config.yaml"
KUBEADM_JOIN_CONFIG_FILE="kubeadm-join-config.yaml"
function create_init_config_yaml() {
cat > $KUBEADM_INIT_CONFIG_FILE <<EOF
---
apiVersion: kubeadm.k8s.io/v1beta3
kind: InitConfiguration
certificateKey: $CERTS_KEY
localAPIEndpoint:
  advertiseAddress: $MASTER_IP
  bindPort: $MASTER_PORT
nodeRegistration:
  imagePullPolicy: IfNotPresent
  name: $(hostname)
  taints:
  - effect: NoSchedule
    key: node-role.kubernetes.io/master

---
apiVersion: kubeadm.k8s.io/v1beta3
kind: ClusterConfiguration
imageRepository: $IMAGE_REGISTRY
kubernetesVersion: v$K8S_VERSION
clusterName: $CLUSTER_NAME
controlPlaneEndpoint: $HA_IP:$HA_PORT
networking:
  podSubnet: $POD_CIDR,$POD_CIDR6
apiServer:
  extraArgs: 
    service-node-port-range: 1024-65535
    default-not-ready-toleration-seconds: "0"
    default-unreachable-toleration-seconds: "0"
    enable-admission-plugins: NodeRestriction,MutatingAdmissionWebhook,ValidatingAdmissionWebhook
    feature-gates: TTLAfterFinished=true,IPv6DualStack=true
controllerManager: 
  extraArgs:
    cluster-cidr: $POD_CIDR,$POD_CIDR6
    node-monitor-grace-period: 30s
    pod-eviction-timeout: 10s
    terminated-pod-gc-threshold: "1"
    feature-gates: TTLAfterFinished=true,IPv6DualStack=true
    node-cidr-mask-size-ipv6: "112"
scheduler:
  extraArgs: 
    feature-gates: TTLAfterFinished=true
dns:

---
apiVersion: kubeproxy.config.k8s.io/v1alpha1
kind: KubeProxyConfiguration
iptables:
  masqueradeAll: false
ipvs:
mode: ipvs
clusterCIDR: $POD_CIDR,$POD_CIDR6

---
apiVersion: kubelet.config.k8s.io/v1beta1
kind: KubeletConfiguration
cgroupDriver: systemd
featureGates:
  IPv6DualStack: true
evictionHard:
  imagefs.available: 10%
  memory.available: 1024Mi
  nodefs.available: 10%
  nodefs.inodesFree: 10%
maxPods: 256
EOF
assert $? "创建kubeadm init配置文件失败"
}

function create_join_config_yaml() {
cat > $KUBEADM_JOIN_CONFIG_FILE <<EOF
apiVersion: kubeadm.k8s.io/v1beta3
kind: JoinConfiguration
EOF

if [[ $NODE_TYPE == master ]]; then
cat >> $KUBEADM_JOIN_CONFIG_FILE <<EOF
controlPlane:
  localAPIEndpoint:
    advertiseAddress: $MASTER_IP
    bindPort: $MASTER_PORT   
  certificateKey: $CERTS_KEY
EOF
fi

cat >> $KUBEADM_JOIN_CONFIG_FILE <<EOF
discovery:
  bootstrapToken:
    apiServerEndpoint: $HA_IP:$HA_PORT
    token: $JOIN_TOKEN
    unsafeSkipCAVerification: true
EOF
assert $? "创建kubeadm join配置文件失败"
}

function prepare_docker() {
	installFlg=0
	installVersion=""

	which dockerd &> /dev/null
	if [[ $? == 0 ]]; then
		curVersion=`docker version | grep -A 1 'Engine:' | grep Version | awk '{print $2}'`
		[[ $DOCKER_VERSION != "" && $curVersion != $DOCKER_VERSION ]] && installFlg=1
	else
		installFlg=1
	fi
	
	if [[ $installFlg == 1 ]]; then
		if [[ $OS_TYPE == ubuntu ]]; then
			apt-cache madison docker-ce | grep ' docker-ce ' > /dev/null
			if [[ $? != 0 ]]; then
				apt-get -y install ca-certificates curl gnupg lsb-release
				assert $?
				
				curl -fsSL https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu/gpg | gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
				#curl -fsSL https://download.docker.com/linux/ubuntu/gpg | gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
				assert $?

				echo  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null
				#echo  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null
				assert $?
				
				info "更新docker源..."
				apt-get -y update
				assert $?
			fi
		
			if [[ $DOCKER_VERSION == "" ]]; then
				installVersion=`apt-cache madison docker-ce | grep docker-ce`
				[[ $installVersion == "" ]] && fatal "未从apt源中发现docker"
				installVersion=`echo $installVersion | head -1 | awk '{print $3}'`
			else
                                osCodeName=`lsb_release -a | grep Codename | awk '{print $2}'`
				installVersion="5:$DOCKER_VERSION~3-0~ubuntu-$osCodeName"
				apt-cache madison docker-ce | grep $installVersion > /dev/null
				[[ $? != 0 ]] && fatal "未从apt源中发现docker版本:$installVersion"
			fi

			which dockerd > /dev/null
			if [[ $? == 0 ]]; then
				info "卸载已安装的docker相关组件..."
                                apt-mark unhold docker-ce docker-ce-cli
				apt-get -y purge docker-ce docker-ce-cli containerd.io
				assert $?
			fi
			
			info "安装docker组件..."
			apt-get -y install docker-ce=$installVersion docker-ce-cli=$installVersion containerd.io
			if [[ $? != 0 ]]; then
				info "请耐心等待1min..."
				sleep 61
				systemctl daemon-reload
				service docker restart
				assert $? "安装docker失败"
			fi		

                        apt-mark hold docker-ce docker-ce-cli
		else
			yum list docker-ce --showduplicates 2> /dev/null | grep "docker-ce-stable" > /dev/null
			if [[ $? != 0 ]]; then
				#yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
				yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
				assert $? "安装docker源失败"

				if [[ $OS_TYPE == bclinux ]]; then
					sed -i 's/$releasever/8/g' /etc/yum.repos.d/docker-ce.repo
					assert $? "修改docker源失败"
				fi
			fi
			
			if [[ $OS_TYPE == bclinux ]]; then
				if [[ ! -e /etc/yum.repos.d/CentOS-Base.repo ]]; then
					wget -O /etc/yum.repos.d/CentOS-Base.repo https://repo.huaweicloud.com/repository/conf/CentOS-8-reg.repo
					assert $? "安装CentOS-Base源失败"

					sed -i 's/$releasever/8/g' /etc/yum.repos.d/CentOS-Base.repo
					assert $? "修改CentOS-Base源失败"
				fi
                                
                                # bclinux默认安装最新版的docker以免报错
                                DOCKER_VERSION="23.0.1"
			fi

			if [[ $DOCKER_VERSION == "" ]]; then
				installVersion=`yum list docker-ce | grep "docker-ce-stable"`
				[[ $installVersion == "" ]] && fatal "未从yum源中发现docker"
				installVersion=`echo $installVersion | head -1 | awk '{print $2}'`
				installVersion=${installVersion##*:}
				installVersion=${installVersion%%-*}
			else
				installVersion="$DOCKER_VERSION"
				yum list docker-ce --showduplicates | grep $installVersion > /dev/null
				[[ $? != 0 ]] && fatal "未从yum源中发现docker版本:$installVersion"
			fi

			which dockerd &> /dev/null
			if [[ $? == 0 ]]; then
				info "卸载已安装的docker相关组件..."
				yum remove -y `yum list installed | grep docker | awk '{print $1}'` containerd.io
				assert $?
			fi
			
			info "安装docker组件..."
			yum install -y docker-ce-$installVersion docker-ce-cli-$installVersion
			assert $? "安装docker失败"
			
			systemctl daemon-reload
			systemctl enable docker.service
			assert $?
		fi
		
		usr=`getent passwd 1000`; usr=${usr%%:*}
		groupadd docker &> /dev/null
		gpasswd -a $usr docker 
		systemctl restart docker.service
		assert $?
	fi

	docker info | grep 'Cgroup Driver' | grep systemd > /dev/null
	if [[ $? != 0 ]]; then
		if [[ -e /etc/docker/daemon.json ]]; then
                        grep 'native.cgroupdriver=cgroup' /etc/docker/daemon.json > /dev/null
                        if [[ $? == 0 ]]; then
                                sed -i 's/native.cgroupdriver=cgroup/native.cgroupdriver=systemd/' /etc/docker/daemon.json
                                assert $? "修改docker配置文件失败"
                        else
                                fatal "请手动配置docker的cgroup driver为systemd，然后重启docker服务"
                        fi
		else
			echo '{"exec-opts": ["native.cgroupdriver=systemd"]}' > /etc/docker/daemon.json
			assert $? "生成docker配置文件失败"
		fi
		
                service docker restart
		assert $? "重启docker服务失败"
	fi
}

function prepare_img() {
	cpuArch=`lscpu | head -1 | awk '{print $2}'`
        [[ $cpuArch == "x86_64" ]] && archl=x86
        [[ $cpuArch == "aarch64" ]] && archl=arm

	work_dir=$(pwd)
        img_dir=${work_dir}/../tar_files
        if [[ -f ${img_dir}/kubernetes_img_${archl}.tar.gz ]]; then
                cd ${img_dir}
                tar zxvf kubernetes_img_${archl}.tar.gz
                cd -
                cd ${img_dir}/kubernetes_img
                imgs=`ls`
                for i in $imgs; do
                        docker load < $i
                done
                cd -
        fi
}

function prepare_k8s() {
	swapoff -a > /dev/null
	assert $?
	grep swap /etc/fstab > /dev/null
	if [[ $? == 0 ]]; then
		sed -i '/swap/d' /etc/fstab > /dev/null
		assert $?
	fi
	
	service firewalld status | grep running > /dev/null
	if [[ $? == 0 ]]; then
		service firewalld stop  > /dev/null
		assert $?
		systemctl disable firewalld > /dev/null
		assert $? 
	fi

	if [[ $OS_TYPE == ubuntu ]]; then
		kubeadmV=`dpkg -l | grep -w kubeadm | awk '{print $3}'`
		if [[ ! $kubeadmV =~ $K8S_VERSION ]]; then
			vInfo=`apt-cache madison kubeadm | grep $K8S_VERSION`
			if [[ $? != 0 ]]; then
				info "更新阿里云k8s源..."
				curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | apt-key add - > /dev/null
				assert $?
				echo "deb https://mirrors.aliyun.com/kubernetes/apt/ kubernetes-xenial main" > /etc/apt/sources.list.d/kubernetes.list
				apt-get -y update
				assert $?

				vInfo=`apt-cache madison kubeadm | grep $K8S_VERSION`
				[[ $? != 0 ]] && fatal "未从apt源中发现该k8s版本"
			fi

			if [[ $kubeadmV != "" ]]; then
                                apt-mark unhold kubeadm kubectl kubelet
				info "卸载已安装的k8s相关组件和工具..."
				apt-get purge -y  kubelet kubeadm kubectl kubernetes-cni
				assert $?
				apt-get -y autoremove
				assert $?
				rm /opt/cni/ -rf
			fi
			
			info "安装k8s相关组件和工具..."
			k8sV=`echo "$vInfo"| head -1 | awk '{print $3}'`

			apt-get install -y kubelet=$k8sV kubeadm=$k8sV kubectl=$k8sV kubernetes-cni
			assert $?
			
			systemctl enable kubelet && systemctl start kubelet
			assert $?
		fi
         
                apt-mark hold kubeadm kubectl kubelet
	else
		getenforce | grep Disabled > /dev/null
		if [[ $? != 0 ]]; then
			setenforce 0 > /dev/null
			assert $?
		fi

		cat /etc/selinux/config | grep "SELINUX=disabled" > /dev/null
		if [[ $? != 0 ]]; then
			sed -i 's/SELINUX=.*/SELINUX=disabled/g' /etc/selinux/config > /dev/null
			assert $?			
		fi	
		
		cat <<EOF >  /etc/sysctl.d/k8s.conf
net.ipv4.ip_forward = 1
net.bridge.bridge-nf-call-ip6tables = 1
net.bridge.bridge-nf-call-iptables = 1
EOF
		sysctl -p /etc/sysctl.d/k8s.conf 
		
		kubeadmV=`yum list installed | grep -w kubeadm | awk '{print $2}'`
		if [[ ! $kubeadmV =~ $K8S_VERSION ]]; then
			yum list --showduplicates | grep kubeadm | grep $K8S_VERSION > /dev/null
			if [[ $? != 0 ]]; then
				arch=x86_64
				lscpu | head -1 | grep aarch64  > /dev/null
				[[ $? == 0 ]] && arch=aarch64
				cat > /etc/yum.repos.d/kubernetes.repo <<EOF
[kubernetes]
name = Kubernetes
baseurl = https://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-$arch
enabled = 1
gpgcheck = 0
repo_gpgcheck = 0
gpgkey = https://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg https://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF
				yum list --showduplicates | grep kubeadm | grep $K8S_VERSION 
				[[ $? != 0 ]] && fatal "未从yum源中发现该k8s版本"
			fi

			if [[ $kubeadmV != "" ]]; then
				info "卸载已安装的k8s相关组件和工具..."
				yum remove -y `yum list installed | grep -E 'kubeadm|kubectl|kubelet|kubernetes-cni' | awk '{print $1}'`
				assert $?
				rm /opt/cni/ -rf
			fi
			
			info "安装k8s相关组件和工具..."
			yum install -y kubelet-$K8S_VERSION kubeadm-$K8S_VERSION kubectl-$K8S_VERSION --disableexcludes=kubernetes
			assert $?
			
			systemctl enable kubelet && systemctl start kubelet
			assert $?
		fi
	fi
}

function on_init_ok() {
	mkdir -p /root/.kube
	cp -rf /etc/kubernetes/admin.conf /root/.kube/config
	
	usr=`getent passwd 1000`; usr=${usr%%:*}
	mkdir -p /home/$usr/.kube
	cp -rf /etc/kubernetes/admin.conf /home/$usr/.kube/config	
	chown $usr:$usr /home/$usr/.kube/config	
	
	kubectl taint node $(hostname) node-role.kubernetes.io/master-
}

function create_cluster_position_configmap() {
        cfgFile=/tmp/cluster-custom-config.yaml
        cat > $cfgFile <<EOF        
longitude: $LONGITUDE
latitude: $LATITUDE
EOF
        kubectl create cm -n kube-public cluster-custom-config --from-file $cfgFile
}

function init() {
	[[ $CUR_NODE_TYPE != "" ]] && fatal "检测到您的机器上已经运行k8s，请使用该脚本执行reset命令后再进行该操作"

	check_ip $MASTER_IP
	add_host_dns $MASTER_IP
	prepare_docker
	prepare_img
	prepare_k8s	
	create_init_config_yaml 
	
	info "初始化集群master节点..."
	kubeadm init --config=$KUBEADM_INIT_CONFIG_FILE --upload-certs #-v=5
	assert $? "初始化集群master节点失败"
	
	on_init_ok

	ok "集群master节点初始化成功"

        create_cluster_position_configmap
        assert $? "集群地位置信息配置失败"

        info "安装集群网络插件..."
        
        np=""; npf=$NETWORK_PLUGIN_FILE
        grep flannel $NETWORK_PLUGIN_FILE > /dev/null
        if [[ $? == 0 ]]; then
		np=flannel
		tmpNpf=$NETWORK_PLUGIN_FILE.tmp
		cp $NETWORK_PLUGIN_FILE $tmpNpf
		tmpV=${POD_CIDR//\//\\\/}
		sed -i "s/.*\"Network\":.*/      \"Network\": \"$tmpV\",/" $tmpNpf
		assert $? "集群POD CIDR设置失败"
		npf=$tmpNpf
        else
                grep calico $NETWORK_PLUGIN_FILE > /dev/null
                [[ $? == 0 ]] && np=calico
        fi
        [[ $np != flannel && $np != calico ]] && fatal "无效的集群网络插件安装文件"

        kubectl apply -f $npf
        assert $? "集群网络插件安装失败"

        ok "集群网络插件安装成功，请运行命令kubectl get pod -A查看集群各组件运行状态是否正常"
}

function reinit() {
	[[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器还未作为master节点在运行，请使用该脚本执行init命令后再进行该操作"
	
	check_ip $MASTER_IP
	add_host_dns $MASTER_IP

	nodeInfo=`timeout 5s kubectl get node -owide 2> /dev/null`
	if [[ $? == 0 ]]; then
		masterIp=`echo "$nodeInfo" | grep master | awk '{print $6}'`
		[[ $masterIp == $MASTER_IP ]] && fatal "当前ip无需更改"
	fi
	
	service kubelet stop > /dev/null
	assert $?

	rm /etc/kubernetes/pki -rf
	rm /etc/kubernetes/*.conf
	rm /etc/kubernetes/manifests/{etcd.yaml,kube-apiserver.yaml}
	rm /var/lib/kubelet/pki -rf
	
	info "重新初始化集群master节点..."
	create_init_config_yaml 
	kubeadm init --config=$KUBEADM_INIT_CONFIG_FILE --skip-phases=preflight --upload-certs #-v=5
	assert $? "集群master节点重新初始化失败"
	
	on_init_ok
	
	info "请耐心等待1min左右......"
	tmpDir=/etc/kubernetes/manifests.tmp
	mkdir -p $tmpDir
	mv /etc/kubernetes/manifests/* $tmpDir/
        
        service kubelet restart
	while true; do
		docker ps | grep -E 'kube-scheduler|kube-controller-manager|kube-apiserver|etcd' > /dev/null
		[[ $? != 0 ]] && break
		sleep 1
	done
	mv $tmpDir/* /etc/kubernetes/manifests/
	rm $tmpDir -rf

        service kubelet restart
	while true; do
		timeout 5s kubectl get pod &> /dev/null
		[[ $? == 0 ]] && break
		sleep 1
	done

	kubectl rollout restart -n kube-system ds kube-proxy 
	assert $? "重启kube-proxy失败"

	kubectl rollout restart -n kube-system ds kube-flannel
	assert $? "重启kube-flannel失败"
	
	ok "集群master节点重新初始化成功"
}

function join() {
	[[ $CUR_NODE_TYPE != "" ]] && fatal "检测到您的机器上已经运行k8s，请使用该脚本执行reset命令后再进行该操作"
	
	if [[ $NODE_TYPE == master ]]; then
	        check_ip $MASTER_IP
		add_host_dns $MASTER_IP
	fi
	if [[ $NODE_TYPE == worker ]]; then
		check_ip $WORKER_IP
		add_host_dns $WORKER_IP
	fi

	prepare_docker
	prepare_img
	prepare_k8s
	
	info "集群加入$NODE_TYPE节点..."
	create_join_config_yaml 
	kubeadm join --config=$KUBEADM_JOIN_CONFIG_FILE #-v=5
	assert $? "集群加入$NODE_TYPE节点失败"
	
	if [[ $NODE_TYPE == master ]]; then
		mkdir -p /root/.kube
		cp -rf /etc/kubernetes/admin.conf /root/.kube/config

		usr=`getent passwd 1000`; usr=${usr%%:*}
		mkdir -p /home/$usr/.kube
		cp -rf /etc/kubernetes/admin.conf /home/$usr/.kube/config	
		chown $usr:$usr /home/$usr/.kube/config	
	fi

	ok "集群加入$NODE_TYPE节点成功"
}

function rejoin() {
	if [[ $NODE_TYPE == master ]]; then
		[[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器还未作为master节点在运行，请使用该脚本执行join命令后再进行该操作"
		check_ip $MASTER_IP
	else
		[[ $CUR_NODE_TYPE == master ]] && fatal "检测到您的机器已经作为master节点正在运行，请使用该脚本执行reset命令后再进行该操作"
                check_ip $WORKER_IP
	fi
	
	systemctl stop kubelet > /dev/null
	assert $?

	rm -rf /var/lib/kubelet/pki /etc/kubernetes/pki /etc/kubernetes/kubelet.conf &> /dev/null

	info "集群重新加入$NODE_TYPE节点..."
	create_join_config_yaml 
	kubeadm join phase kubelet-start --config=$KUBEADM_JOIN_CONFIG_FILE #-v=5
	assert $? "集群重新加入$NODE_TYPE节点失败"
	
	[[ $NODE_TYPE == master ]] && on_init_ok

	ok "集群重新加入$NODE_TYPE节点成功"
}

function reset() {
	while true; do
		read -p "该操作将会删除集群k8s的etcd数据(master节点)和已创建的节点资源数据，请确认是否继续(y/n):" i
		[[ $i == "y" ]] && break
		[[ $i == "n" ]] && return 0
	done
	
	info "开始清除k8s运行环境..."

	kubeadm reset -f #-v=5
	assert $?

	rm /etc/cni/ -rf &> /dev/null
	rm /var/lib/cni/ -rf &> /dev/null
	rm /var/run/calico/ -rf &> /dev/null
	ip link del flannel.1 &> /dev/null
	ip link del cni0 &> /dev/null
	ip link del kube-ipvs0 &> /dev/null
	modprobe -r ipip &> /dev/null

	ok "k8s运行环境清除成功，建议将服务器重启"
}

function upgrade() {
        [[ $DOCKER_UPGRADE_FLG == 1 ]] && prepare_docker

        if [[ $K8S_UPGRADE_FLG == 1 ]]; then
                if [[ $NODE_TYPE == master ]]; then
                        [[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器没有作为master节点在运行，无法升级"	
                        if [[ $OS_TYPE == ubuntu ]]; then
                                apt-mark unhold kubeadm
                                apt-get install -y kubeadm=$K8S_VERSION-00 --allow-downgrades                     
                                assert $? "kubeadm更新失败"
                                apt-mark hold kubeadm 
                                
                                kubeadm upgrade apply v$K8S_VERSION
                                assert $? "k8s更新失败"

                                apt-mark unhold kubelet kubectl
                                apt-get install -y kubelet=$K8S_VERSION-00 kubectl=$K8S_VERSION-00 --allow-downgrades
                                assert $? "kubelet，kubectl更新失败"
                                apt-mark hold kubectl kubelet
			else
				yum install -y kubeadm-$K8S_VERSION --disableexcludes=kubernetes
				assert $? "kubeadm更新失败"
                                
				kubeadm upgrade apply v$K8S_VERSION
                                assert $? "k8s更新失败"
                                
				yum install -y kubelet-$K8S_VERSION kubectl-$K8S_VERSION kubernetes-cni --disableexcludes=kubernetes
                                assert $? "kubelet，kubectl更新失败"
			fi
                else
                        [[ $CUR_NODE_TYPE == master ]] && fatal "检测到您的机器正作为master节点在运行，无法升级"
                        if [[ $OS_TYPE == ubuntu ]]; then
                                apt-mark unhold kubeadm
                                apt-get install -y kubeadm=$K8S_VERSION-00 --allow-downgrades                     
                                assert $? "kubeadm更新失败"
                                apt-mark hold kubeadm
                                
                                kubeadm upgrade node
                                assert $? "k8s更新失败"

                                apt-mark unhold kubelet kubectl
                                apt-get install -y kubelet=$K8S_VERSION-00 kubectl=$K8S_VERSION-00 --allow-downgrades
                                assert $? "kubelet，kubectl更新失败"
                                apt-mark hold kubectl kubelet
			else
				yum install -y kubeadm-$K8S_VERSION --disableexcludes=kubernetes
				assert $? "kubeadm更新失败"
                                
				kubeadm upgrade node
                                assert $? "k8s更新失败"
                                
				yum install -y kubelet-$K8S_VERSION kubectl-$K8S_VERSION kubernetes-cni --disableexcludes=kubernetes
                                assert $? "kubelet，kubectl更新失败"
			fi
                fi
		
		systemctl daemon-reload
		assert $?
		systemctl restart kubelet
		assert $?
        fi
}


function status() {
        [[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器没有作为master节点在运行，无法创建token"
        echo -e "=============== \033[34m各节点pod容器状态\033[0m ==============="
        kubectl get pod -n kube-system -owide
        echo ""
        echo -e "=============== \033[34m各节点kubelet状态\033[0m ==============="
        kubectl get node -owide
        echo ""
}

function create_token() {
        [[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器没有作为master节点在运行，无法创建token"
        echo -e "=============== \033[34mjoin token\033[0m ==============="
        kubeadm token create
        echo ""
}

function check_certs() {
        [[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器没有作为master节点在运行，无法查看证书信息"
        echo -e "=============== \033[34m证书信息\033[0m ==============="
        kubeadm  certs check-expiration
        echo ""
}

function update_certs() {
	[[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器没有作为master节点在运行，无法进行证书更新"	
        cp -a /etc/kubernetes/ /etc/kubernetes-`date +%Y%m%d`
        kubectl get cm kubeadm-config -n kube-system -o yaml > /root/kubeadm-config.yaml        
        
        info "生成新证书..."
        kubeadm certs renew all
        assert $? "新证书生成失败"
        
        on_init_ok 

        info "重启docker服务启用新证书..."
        service docker restart
        assert $? "重启docker失败"
        
        info "等待k8s组件服务启动..."
	while true; do
		timeout 5s kubectl get pod &> /dev/null
		[[ $? == 0 ]] && break
		sleep 1
	done
        
        ok "证书更新成功"
}

function backup_data() {
	[[ $CUR_NODE_TYPE != master ]] && fatal "检测到您的机器没有作为master节点在运行，无法进行etcd数据备份"
	
	which etcdctl > /dev/null
	if [[ $? != 0 ]]; then
		if [[ $OS_TYPE == ubuntu ]]; then
			apt-get install -y etcd-client
		else
			yum install -y etcd
		fi
		assert $? "etcd-client安装失败"
	fi

	ETCDCTL_API=3 etcdctl \
		snapshot save $DATA_FILE \
		--endpoints=https://127.0.0.1:2379 \
		--cacert=/etc/kubernetes/pki/etcd/ca.crt \
		--cert=/etc/kubernetes/pki/etcd/peer.crt \
		--key=/etc/kubernetes/pki/etcd/peer.key
	assert $? "k8s etcd数据备份失败"

	ok "k8s etcd数据备份成功"
}

function restore_data() {
	info "停止kubelet服务..."
	service kubelet stop > /dev/null
	assert $? "停止kubelet服务失败"
	
	info "停止kube-apiserver和etcd容器..."
	docker stop `docker ps | grep -E 'kube-apiserver|etcd' | awk '{print $1}'` > /dev/null
	assert $? "停止kube-apiserver和etcd容器失败"
	
	mv /var/lib/etcd/ /var/lib/etcd.bak.$(date +%F_%H:%M:%S)
	
	which etcdctl > /dev/null
	if [[ $? != 0 ]]; then
		if [[ $OS_TYPE == ubuntu ]]; then
			apt-get install -y etcd-client
		else
			yum install -y etcd
		fi
		assert $? "etcd-client安装失败"
	fi
	
	info "恢复etcd数据..."
	ETCDCTL_API=3 etcdctl \
		snapshot restore $DATA_FILE \
		--data-dir=/var/lib/etcd > /dev/null
	assert $? "k8s etcd数据恢复失败"

	service kubelet restart > /dev/null
	
	info "请耐心等待30s左右......"
	while true; do
		kubectl cluster-info &> /dev/null
		[[ $? == 0 ]] && break
		sleep 1
	done

	ok "k8s etcd数据恢复成功"
}

function main() {
	get_os_info
        get_node_type
	process_cmdline_params "$@"

        ls /root &> /dev/null
	assert $? "必须使用root权限执行该脚本"

	if [[ $CMD == "init" ]]; then
		init
	elif [[ $CMD == "reinit" ]]; then
		reinit
	elif [[ $CMD == "join" ]]; then
		join
	elif [[ $CMD == "rejoin" ]]; then
		rejoin
	elif [[ $CMD == "reset" ]]; then
		reset
	elif [[ $CMD == "upgrade" ]]; then
		upgrade
	elif [[ $CMD == "status" ]]; then
		status
	elif [[ $CMD == "create-token" ]]; then
		create_token
	elif [[ $CMD == "check-certs" ]]; then
		check_certs
	elif [[ $CMD == "update-certs" ]]; then
		update_certs
	elif [[ $CMD == "backup-data" ]]; then
		backup_data
	elif [[ $CMD == "restore-data" ]]; then
		restore_data
	else
		fatal "无效的命令"
	fi
}
main "$@"
exit $?














