#!/bin/bash

[ ${UID} -ne 0 ] && echo "Please run with sudo" && exit -1

SDGS_NVIDIA_VERSION="v1.0.1"
SDGS_INTEL_VERSION="v1.0.1"

RDMA_DRIVER_NAME="MLNX_OFED_LINUX-5.3-1.0.0.1-rhel7.8-x86_64"
RDMA_DRIVER_PACKAGE="${RDMA_DRIVER_NAME}.tgz"
RDMA_DRIVER_URL="https://content.mellanox.com/ofed/MLNX_OFED-5.3-1.0.0.1/${RDMA_DRIVER_PACKAGE}"

NV_DRIVER_NAME="NVIDIA-Linux-x86_64-470.57.02.run"
NV_DRIVER_URL="https://cn.download.nvidia.cn/XFree86/Linux-x86_64/470.57.02/${NV_DRIVER_NAME}"

SG1_DRIVER_NAME="SG1_20132_c7.4_k4.14.105_internal"
SG1_DRIVER_PACKAGE="$SG1_DRIVER_NAME.7z"

LOCAL_RDMA_DRIVER_DIR="drivers/rdma"
LOCAL_NVDIA_DRIVER_DIR="drivers/video/nvidia"
LOCAL_INTEL_DRIVER_DIR="drivers/video/intel"
LOCAL_SDGS_DIR="packages/rpm"

OS_VERSION_ID=$(grep "VERSION_ID=" /etc/*-release | awk -F "=" '{print $2}')
OS_VERSION_ID=$(echo $OS_VERSION_ID | sed 's/\"//g')
ARCH=$(uname -m)
KERNEL_RELEASE=$(uname -r)
KERNEL_SRC_PATH="/usr/src/kernels/${KERNEL_RELEASE}/"
TIME=$(date +%Y%m%d)
DOWNLOAD_DEPEND_PACKAGE_DIR="sdgs_depend_rpm_packages"
DEP_GPU_TYPE=""

function error() {
    echo -e "\e[01;31m[Error]: $*\e[0m"
}

function waring()
{
    echo -e "\e[01;33m$*\e[0m"
}

function info() {
    echo -e "\e[01;32m$*\e[0m"
}

function out() {
    echo -e "$*"
}

function install_rdma_driver()
{
    info "Install RDMA driver."

    lspci | grep "Ethernet" | grep "Mellanox" > /dev/null
    if [ $? -ne 0 ]; then
        waring "RDMA network card (Mellanox) not detected."
    fi

    info "Check whether the driver is installed"
    local driver_ver=$(rpm -qa | grep mlnx-fw-updater | awk -F "-" '{print $4"-"$5}')
    driver_ver=$(echo ${driver_ver%.*})
    echo "$RDMA_DRIVER_PACKAGE" | grep "$driver_ver" > /dev/null
    if [ $? -eq 0 ] && [ ! -z $driver_ver ]; then
        out "   RDMA network card driver has been installed. version=$driver_ver"
        return 0
    fi

    if [ ! -e "${LOCAL_RDMA_DRIVER_DIR}/${RDMA_DRIVER_PACKAGE}" ]; then
        out "rdma url: $RDMA_DRIVER_URL"
        wget $RDMA_DRIVER_URL
    fi

    local rdma_driver="${LOCAL_RDMA_DRIVER_DIR}/${RDMA_DRIVER_PACKAGE}"
    if [ -e "$rdma_driver" ]; then
        tar zxvf $rdma_driver
        pushd "$PWD"
        cd $RDMA_DRIVER_NAME
        grep "$KERNEL_RELEASE" .supported_kernels > /dev/null
        if [ $? -eq 0 ]; then
            info "RDMA driver supports the current system, install directly, wait..."
            sudo ./mlnxofedinstall --force --skip-unsupported-devices-check --skip-repo
            if [ $? -ne 0 ]; then
                error "rdma driver install failed."
                return 112
            fi
        else
            info "RDMA driver does not support the current system, it needs to be compiled and then installed, wait..."
            sudo ./mlnxofedinstall --force --skip-unsupported-devices-check --skip-repo --add-kernel-support
            if [ $? -ne 0 ]; then
                error "rdma driver build and install failed."
                return 112
            fi
        fi
        info "Restart openibd service"
        sudo /etc/init.d/openibd restart
        popd
    else
        error "RDMA driver installation package does not exist. $rdma_driver"
        return 110
    fi

    info "Check RDMA driver:"
    ibdev2netdev
    if [ $? -eq 0 ]; then
        info "RDMA driver installed successfully"
    else
        error "RDMA driver installation is abnormal, please check.."
        return 111
    fi

    local rdma_array=($(echo `ibdev2netdev | grep "mlx5" | grep Up | awk '{print $1}'`| sed 's/\n//g'))

    for rdma_dev in "${rdma_array[@]}"
    do
        ibstatus "$rdma_dev" | grep "Ethernet" > /dev/null
        if [ $? -ne 0 ]; then
            error "The current RDMA network card ($rdma_dev) mode does not support, only supports Ethernet mode."
            return 113
        fi
    done
}

function check_ip()
{
    local ip=$1
    info "Check ip: $ip"
    local valid_check=$(echo $ip | awk -F. '$1<=255&&$2<=255&&$3<=255&&$4<=255{print "yes"}')
    if echo $ip | grep -E "^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$">/dev/null; then
        if [ "${valid_check:-no}" == "yes" ]; then
            echo "IP $ip available."
        else
            error "IP($ip) not available!"
            return 255
        fi
    else
        error "IP($ip) format error!"
        return 255
    fi
}

function config_rdma_net_ip()
{
    info "Configure RDMA network IP:"

    local rdma_array=($(echo `ibdev2netdev | grep "mlx5" | grep Up | awk '{print $5}'`| sed 's/\n//g'))

    local rdma_dev_sum=${#rdma_array[@]}
    if [ "$rdma_dev_sum" -eq 0 ]; then
        error "There is currently no available network card, please check whether the network cable is connected."
        return 0
    fi

    local net_dev=${rdma_array[0]}

    local net_dev_cfg="/etc/sysconfig/network-scripts/ifcfg-${net_dev}"

    check_ip "$__RDMA_NETWORK_IP"
    if [ $? -ne 0 ]; then
        return 255
    fi

    out "RDMA netwoek device: $net_dev"

    out "   config file: $net_dev_cfg"

    sudo sed -i "/^BOOTPROTO/c BOOTPROTO=static" "${net_dev_cfg}"
    sudo sed -i "/^ONBOOT/c ONBOOT=yes" "${net_dev_cfg}"

    grep "IPADDR" "$net_dev_cfg" >> /dev/null
    if [ $? -ne 0 ]; then
        sudo sed -i "a IPADDR=$__RDMA_NETWORK_IP" "${net_dev_cfg}"
    else
        sudo sed -i "/^IPADDR/c IPADDR=$__RDMA_NETWORK_IP" "${net_dev_cfg}"
    fi

    grep "NETMASK" "$net_dev_cfg" >> /dev/null
    if [ $? -ne 0 ]; then
        sudo sed -i "a NETMASK=255.255.255.0" "${net_dev_cfg}"
    fi

    sudo systemctl restart network

    ifconfig "$net_dev"
}

function is_reboot()
{
    info "Need to restart the system ..."

    echo -n "Do you want to reboot? [Y/n]"
    read enter_x
    if [ "x$enter_x" == "xy" ] || [ "x$enter_x" == "xY" ]; then
        waring "******** Restart Now ********"
        sudo reboot
    fi
    return 255
}

function disable_nouveau_driver()
{
    info "Remove nouveau driver."

    local DIST_BLACKLIST="/lib/modprobe.d/dist-blacklist.conf"

    sudo sed -i "s/blacklist nvidiafb/#blacklist nvidiafb/g" $DIST_BLACKLIST
    sudo sed -i '$a blacklist nouveau' $DIST_BLACKLIST
    sudo sed -i '$a options nouveau modeset=0' $DIST_BLACKLIST

    info "Dracut rebuild the initramfs image, Wait..."
    sudo mv /boot/initramfs-"$KERNEL_RELEASE".img /boot/initramfs-"$KERNEL_RELEASE".img.bak
    sudo dracut /boot/initramfs-"$KERNEL_RELEASE".img "$KERNEL_RELEASE"

    waring "**** The system must be restarted to uninstall the nouveau driver ****"
    is_reboot
}

function check_nvidia_driver()
{
    info "Check Nvidia GPU driver"

    lsmod | grep "nvidia_drm" > /dev/null
    if [ $? -ne 0 ]; then
        out "   Nvidia driver is not installed"
        return 0
    fi

    local nv_version=$(grep "NVRM version" /proc/driver/nvidia/version | awk '{print $8}')

    echo "$NV_DRIVER_NAME" | grep "$nv_version" > /dev/null
    if [ $? -eq 0 ]; then
        out "   The current Nvidia driver version has been installed."
        return 1
    fi

    info "Uninstall nvidia driver, version=$nv_version"
    sudo nvidia-uninstall
    if [ $? -ne 0 ]; then
        error "Uninstall nvidia driver failed."
        return 255
    fi

    waring "**** The system must be restarted, Uninstall the nvidia driver version that does not meet the requirements ****"
    is_reboot
    return $?
}

function install_nvidia_gpu_driver()
{
    info "Install Nvidia GPU driver."

    info "Check nouveau driver:"
    lsmod | grep nouveau > /dev/null
    if [ $? -eq 0 ]; then
        disable_nouveau_driver
    else
        out "   nouveau driver does not exist."
    fi

    info "Check whether the Nvidia driver is installed:"
    check_nvidia_driver
    local ret=$?
    if [ $ret -ne 0 ]; then
        if [ $ret -eq 1 ]; then
            return 0
        fi
        return 255
    fi

    info "Install Nvidia driver:"
    if [ ! -e "${LOCAL_NVDIA_DRIVER_DIR}/${NV_DRIVER_NAME}" ]; then
        out "Nvidia URL: $NV_DRIVER_URL"
        wget $NV_DRIVER_URL
    fi
    if [ ! -d "$KERNEL_SRC_PATH" ]; then
        error "Unable to find the kernel source tree for the currently running kernel($KERNEL_RELEASE).\n
		KERNEL_SRC_PATH=$KERNEL_SRC_PATH does not exist"
        return 255
    fi
    local nv_driver="${LOCAL_NVDIA_DRIVER_DIR}/${NV_DRIVER_NAME}"
    if [ -e "$nv_driver" ]; then
        chmod +x $nv_driver
        #sudo ./$nv_driver --no-x-check --kernel-source-path=${KERNEL_SRC_PATH}
        sudo ./$nv_driver --no-x-check
        if [ $? -ne 0 ]; then
            error "Nvidia GPU driver install failed."
            return 112
        fi
    else
        error "Nvidia driver installation package does not exist. $nv_driver"
        return 120
    fi

    info "Check Nvidia driver, Wait..."
    nvidia-smi > /dev/null
    if [ $? -eq 0 ]; then
        info "Nvidia driver installed successfully"
    else
        error "Nvidia driver installation is abnormal, please check.."
        return 121
    fi
}

function install_intel_sg1_driver()
{
    info "Install Intel SG1 driver."

    local gpu_node_sum=$(lspci | grep "VGA" | grep "4907" | wc -l)

    if [ $((gpu_node_sum % 4)) -ne 0 ]; then
        waring "SG1 GPU rendering node($gpu_node_sum) loading is incomplete!"
    fi

    info "Check whether the SG1 driver is installed:"
    vainfo 2>&1 | grep "SG1" > /dev/null
    if [ $? -eq 0 ]; then
        local i915_ver=$(cat /sys/module/i915/srcversion)
        out "i915_ver=${i915_ver}, SG1.D5A9880B5EADD4EBF6BB5B9"
        out "   The current SG1 driver version has been installed."
        return 0
    else
        out "   SG1 driver is not installed"
    fi

    if [ ! -e "${LOCAL_INTEL_DRIVER_DIR}/${SG1_DRIVER_PACKAGE}" ]; then
        error "The driver installation package does not exist, please check the file name or driver version."
        return 255
    fi

    info "Unzip the installation package:"
    7za x ${LOCAL_INTEL_DRIVER_DIR}/$SG1_DRIVER_PACKAGE

    pushd "$PWD"
    cd $SG1_DRIVER_NAME
    chmod +x install-sg1.sh
    info "Install..."
    sudo ./install-sg1.sh
    if [ $? -ne 0 ]; then
        error "Intel SG1 driver install failed."
        return 112
    fi
    popd
    info "SG1 driver installed successfully"

    local boot_kernel="4.14.105"
    info "Configure default startup items:(kernel ${boot_kernel})"
    local grub2_file="/etc/grub2.cfg"
    local grub2_efi_file="/etc/grub2-efi.cfg"

    if [ -f $grub2_efi_file ];then
        grub2_file=$grub2_efi_file
    fi
    out "grub2 conf: $grub2_file"

    local boot_item=$(awk -F\' '$1=="menuentry " {print i++ " : " $2}' $grub2_file | grep "${boot_kernel}" | awk -F: '{print $1}')
    out "kernel ${boot_kernel} item: [${boot_item}]"

    if [ $boot_item ]; then
        out "Set grub2-set-default ${boot_item}"
        grub2-set-default ${boot_item}
    fi

    is_reboot
    return $?
}

function config_firewalld()
{
    info "Configure firewalld."

    if [ -z "$__RDMA_NETWORK_IP" ]; then
        info "Disable firewalld"
        sudo systemctl stop firewalld.service
        sudo systemctl disable firewalld.service
    else
        local rdma_network_smg=$(echo "$__RDMA_NETWORK_IP" | awk -F . '{print $1"."$2"."$3".0"}')
        info "firewalld filter network segment: ip:[$__RDMA_NETWORK_IP], smg: [$rdma_network_smg]"
        #sudo iptables -F
        #sudo iptables -A INPUT -p tcp --dport 5555  -j ACCEPT
        #sudo iptables -A INPUT -s "$rdma_network_smg"/24 -d "$__RDMA_NETWORK_IP" -j ACCEPT
        #sudo iptables-save | sudo tee /etc/sysconfig/iptables > /dev/null
        sudo systemctl start firewalld.service
        sudo firewall-cmd --permanent --add-port=5555/tcp
        sudo firewall-cmd --permanent --add-rich-rule="rule family="ipv4" source address="$rdma_network_smg/24" accept"
        # for R
        sudo firewall-cmd --permanent --add-port=5061-"$((5061+100))"/tcp
        sudo firewall-cmd --permanent --add-port=11120-"$((11120+40*100))"/udp
        sudo firewall-cmd --reload
        sudo systemctl restart firewalld.service
        sudo systemctl enable firewalld.service
        out "firewall-cmd --list-all"
        sudo firewall-cmd --list-all
    fi
}

function install_depend_sw_for_rdma()
{
    info "Install RDMA depend sw."

    #sudo ${SW_INSTALL_CMD} perl perl-devel
    # for Install RDMA driver
    sudo ${SW_INSTALL_CMD} pciutils gcc-gfortran tcsh libusbx lsof tcl fuse-libs tk
    # for Build RDMA driver
    sudo ${SW_INSTALL_CMD} python-devel redhat-rpm-config rpm-build
    sudo ${SW_INSTALL_CMD} automake libtool
}

function install_depend_sw_for_nvidia()
{
    info "Install Nvidia GPU depend sw."
    sudo ${SW_INSTALL_CMD} kernel-devel
}

function install_depend_sw_for_sg1()
{
    info "Install SG1 GPU depend sw."

    sudo ${SW_INSTALL_CMD} p7zip
}


function install_sdgs_rpm_sw()
{
    info "Install SDG server sw package.."

    local sdgs_package_name="sdg-nvidia-server"
    local sdgs_version=${SDGS_NVIDIA_VERSION}

    lspci | grep "VGA" | grep "Intel" | grep "4907" > /dev/null
    if [ $? -eq 0 ]; then
        sdgs_package_name="sdg-intel-server"
        sdgs_version=${SDGS_INTEL_VERSION}
    fi

    local sdgs_sw_path="$LOCAL_SDGS_DIR/${sdgs_package_name}-${sdgs_version}-1.el${OS_VERSION_ID}.${ARCH}.rpm"

    out "sdgs sw; $sdgs_sw_path"
    if [ ! -e $sdgs_sw_path ]; then
        error "$sdgs_sw_path package does not exist."
        return 255
    fi

    sudo rpm -qa | grep "${sdgs_package_name}-${sdgs_version}" > /dev/null
    if [ $? -eq 0 ]; then
        info "${sdgs_package_name}-${sdgs_version} package has been installed."
        return 0
    fi

    sudo rpm -qa | grep "${sdgs_package_name}" > /dev/null
    if [ $? -eq 0 ]; then
        info "$sdgs_package_name package has other versions installed, uninstall..."
        sudo yum remove -y ${sdgs_package_name}
    fi
    sudo ${SW_INSTALL_CMD} $sdgs_sw_path
}

function install_base_depend_sw()
{
    info "Install base depend software."

    if [ "$__ONLY_DOWNLOAD_DEPEND" = "true" ]; then
        info "   RPM packages download path: ${PWD:?}/$DOWNLOAD_DEPEND_PACKAGE_DIR"
        SW_INSTALL_CMD="yum -y install --downloadonly --downloaddir=${PWD:?}/$DOWNLOAD_DEPEND_PACKAGE_DIR"
        rm ${PWD:?}/$DOWNLOAD_DEPEND_PACKAGE_DIR -rf
    else
        SW_INSTALL_CMD="yum -y install"
    fi

    out "SW_INSTALL_CMD: $SW_INSTALL_CMD"

    sudo ${SW_INSTALL_CMD} elrepo-release epel-release
    sudo ${SW_INSTALL_CMD} pciutils #lspci
    sudo ${SW_INSTALL_CMD} net-tools bash-completion wget
    if [ $? -ne 0 ]; then
        return 255
    fi

    install_depend_sw_for_rdma
    if [ $? -ne 0 ]; then
        return 255
    fi
    lspci | grep "VGA" | grep "NVIDIA" > /dev/null
    if [ $? -eq 0 ]; then
        DEP_GPU_TYPE="nvidia"
        install_depend_sw_for_nvidia
        if [ $? -ne 0 ]; then
            return 255
        fi
    fi
    lspci | grep "VGA" | grep "Intel" | grep "4907" > /dev/null
    if [ $? -eq 0 ]; then
        DEP_GPU_TYPE="SG1"
        install_depend_sw_for_sg1
        if [ $? -ne 0 ]; then
            return 255
        fi
    fi
    install_sdgs_rpm_sw
    if [ $? -ne 0 ]; then
        return 255
    fi
}

function enable_journal()
{
    info "[journal] logs save ..."

    sudo mkdir -p /var/log/journal
    sudo systemctl restart systemd-journald
}

USAGE=$(cat <<- EOF
    Version : 0.1.0
    This script is used to assist setup SDG server enviroment on Centos7.8
        Only supports Nvidia and Intel SG1 graphics cards

    Usage: ./env_setup.sh [OPTIONS]

    Options:
        --only-download-depend  Download only dependent packages
        --rdma-ip <ip_addr>     Configure RDMA network card ip address
        --help                  Help.

    Example:
        ./env_setup.sh
        ./env_setup.sh --only-download-depend
        ./env_setup.sh --rdma-ip 192.168.2.2

EOF
)

function parse_args()
{
    while [[ $# -gt 0 ]]; do
        case $1 in
            --only-download-depend)
                __ONLY_DOWNLOAD_DEPEND="true"
                ;;
            --rdma-ip)
                shift
                if [ "$#" -lt 1 ]; then
                    echo "$USAGE"
                    error "--rdma-ip need to with a IP address."
                    exit -1
                fi
                __RDMA_NETWORK_IP=$1
                shift
                ;;
            --help)
                echo "$USAGE"
                exit
                ;;
            *)
                error "Unrecognised parameter $1"
                echo "$USAGE"
                return 255
                ;;
        esac
        shift
    done

    return 0
}

function main()
{
    local is_install_gpu_driver=0

    info "Build SDG server env."

    grep "CentOS" /etc/*-release > /dev/null
    if [ $? -ne 0 ]; then
        error "The current system does not support, currently only supports CentOS."
        return 255
    fi

    parse_args "$@"
    if [ $? -ne 0 ]; then
        return 210
    fi

    install_base_depend_sw
    if [ $? -ne 0 ]; then
        error "Base depend software installation failed. please check the network or package name..."
        return 216
    fi

    if [ "$__ONLY_DOWNLOAD_DEPEND" = "true" ]; then
        if [ "`ls -A $DOWNLOAD_DEPEND_PACKAGE_DIR`" != "" ]; then
            tar zcvf "${DOWNLOAD_DEPEND_PACKAGE_DIR}_${DEP_GPU_TYPE}_${TIME}_el${OS_VERSION_ID}.${ARCH}".tar.gz $DOWNLOAD_DEPEND_PACKAGE_DIR
        fi
        info "Dependent package download is complete.."
        exit
    fi

    lspci | grep "VGA" | grep "NVIDIA" > /dev/null
    if [ $? -eq 0 ]; then
        if [ ! -d "$KERNEL_SRC_PATH" ]; then
            info "The kernel-devel version is inconsistent with the current kernel version, upgrade the kernel."
            sudo yum install -y  kernel kernel-headers
            waring "The current kernel version has been upgraded and needs to be restarted."
            is_reboot
            return 255
        fi
        info "Currently using Nvidia graphics card, install the driver."
        install_nvidia_gpu_driver
        if [ $? -ne 0 ]; then
            return 211
        fi
        is_install_gpu_driver=1
    fi

    lspci | grep "VGA" | grep "Intel" | grep "4907" > /dev/null
    if [ $? -eq 0 ]; then
        info "Currently using Intel (SG1) graphics card, install the driver."
        install_intel_sg1_driver
        if [ $? -ne 0 ]; then
            return 212
        fi
        is_install_gpu_driver=1
    fi

    if [ $is_install_gpu_driver -eq 0 ]; then
        error "No valid graphics card driver is installed."
        error "please check the hardware environment or installation script.."
        lspci | grep "VGA"
        return 213
    fi

    install_rdma_driver
    if [ $? -ne 0 ]; then
        return 214
    fi

    if [ ! -z "$__RDMA_NETWORK_IP" ]; then
        config_rdma_net_ip
        if [ $? -ne 0 ]; then
            return 215
        fi
    fi

    config_firewalld
    enable_journal

    info "###################################"
    info "###                             ###"
    info "###  SDG Server deply complete  ###"
    info "###                             ###"
    info "###################################"
    waring "#          Last reboot            #"
    is_reboot
}

main "$@"
