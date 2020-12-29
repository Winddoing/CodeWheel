#!/bin/bash
##########################################################
# File Name		: build_x_server.sh
# Author		: winddoing
# Created Time	: 2020年12月25日 星期五 13时39分34秒
# Description	:
##########################################################

X_DEV_DRIVER=""
X_DEV_BUS_ID=()
X_CONFIG_PATH="/etc/X11/"
X_SERVICE_PATH="/usr/lib/systemd/system/"
ALLLOG="/tmp/sdg_env_$(date +%F-%H).log"
FAILED=255

function warning() {
    echo -e "[$(date +%T)] $@" >> $ALLLOG
    echo -e "\e[01;31m $@ \e[0m"
}

function out() {
    echo -e "[$(date +%T)] $@" >> $ALLLOG
    if [[ "$QUIET" == "1" ]]; then return; fi
    echo -e "$@"
}

function debug() {
    echo -e "[$(date +%T)] $@" >> $ALLLOG
    if [[ "$DEBUG" != "1" ]]; then return; fi
    echo -e "$@"
}

function help() {
    echo "Usage:"
    echo -e "\t$0 nvidia|intel"
}

function parse_args()
{
    if [[ $# -lt 1 ]] || [[ $# -gt 11 ]]; then
        warning "Too many arguments"
        help
        return $FAILED
    fi

    while [[ $# -gt 0 ]]; do
        case $1 in
            nvidia)
                NVIDIA=true
                X_DEV_DRIVER="nvidia"
                DEV_FILTER_STR="NVIDIA"
                ;;
            intel)
                INTEL=true
                X_DEV_DRIVER="modesetting"
                DEV_FILTER_STR="Intel"
                ;;
            amd)
                AMD=true
                X_DEV_DRIVER="amdgpu"
                DEV_FILTER_STR="AMD"
                ;;
            -d|--debug)
                DEBUG=1
                ;;
            -q|--quiet)
                QUIET=1
                ;;
            -c|--clean)
                CLEAN=1
                ;;
            *)
                warning "Unrecognised parameter $1"
                return $FAILED
                ;;
        esac
        shift
    done

    if [[ "$NVIDIA" != "true" ]] && [[ "$INTEL" != "true" ]] \
        && [[ "$AMD" != "true" ]] ; then
            warning "Specify the type of graphics card"
            help
            return $FAILED
    fi
}

function vga_devices_info() {
    out "Analyze VGA device info ..."

    local lspci_cmd="lspci"
    debug "`$lspci_cmd | grep VGA | grep ${DEV_FILTER_STR}`"

    local lspci_bus_addr=`$lspci_cmd | grep VGA | grep ${DEV_FILTER_STR} | awk '{print $1}'`
    lspci_bus_addr=`echo $lspci_bus_addr | sed 's/\n//g'`

    local i=0
    local pci_bus_array=($lspci_bus_addr)
    local bus_id_num=${#pci_bus_array[@]}
    debug "\tThe total number of video cards is [$bus_id_num]"
    if [ $bus_id_num -eq 0 ]; then
        warning "[Error] no $DEV_FILTER_STR video card devices."
        return 0
    fi

    for bus_id in ${pci_bus_array[@]}
    do
        bus_id=`echo $bus_id | sed 's/\./:/g'`

        local bus_id_a=`echo $bus_id | awk -F ':' '{print $1}'`
        local bus_id_b=`echo $bus_id | awk -F ':' '{print $2}'`
        local bus_id_c=`echo $bus_id | awk -F ':' '{print $3}'`

        bus_id_dec=`echo "$((16#$bus_id_a)):$((16#$bus_id_b)):$((16#$bus_id_c))"`

        debug "\tnumeric convertion [$i]: $bus_id -(16to10)-> $bus_id_dec"
        X_DEV_BUS_ID[i]=$bus_id_dec
        i=$((i + 1))
    done

    return $bus_id_num
}

function generate_xorg_conf() {
    local x_conf_id=`expr $1 + 1`
    local device_driver=$2
    local device_busid="PCI:$3"
    local xorg_conf_file="${X_CONFIG_PATH:-$PWD}/xorg-$x_conf_id.conf"

    debug "\tgenerate_xorg_conf: x_conf_id=$x_conf_id"
    debug "\tdevice_driver=$device_driver, device_busid=$device_busid"
    debug "\txorg_conf_file=$xorg_conf_file"
    cat > $xorg_conf_file << EOF
Section "ServerLayout"
	Identifier     "X.org Configured"
	Screen      0  "Screen0" 0 0

	Option      "DontVTSwitch"      "on"
	Option      "AutoAddDevices"    "off"
	Option      "AutoEnableDevices" "off
	Option      "AutoAddGPU"        "off"
	Option      "AutoBindGPU"       "off"
EndSection

Section "Monitor"
	Identifier   "Monitor0"
	VendorName   "Monitor Vendor"
	ModelName    "Monitor Model"
EndSection

Section "Device"
	Identifier  "Card0"
	Driver      "$device_driver"
	BusID       "$device_busid"

    `if [[ "$NVIDIA" == "true" ]]; then \
        echo -e "\t#for NVIDIA Xorg configure"
        #https://download.nvidia.com/XFree86/Linux-x86_64/304.137/README/configtwinview.html
        echo -e "\tOption      \"ConnectedMonitor\"  \"DFP-0\""
        echo -e "\tOption      \"MetaModes\"         \"DFP-0: 1920x1080\""
        echo -e "\tOption      \"HorizSync\"         \"DFP-0: 40-70\""
        echo -e "\tOption      \"VertRefresh\"       \"DFP-0: 60\""
        #http://download.nvidia.com/XFree86/Linux-x86_64/180.27/README/appendix-b.html
        echo -e "\tOption      \"ConnectToAcpid\"    \"false\""
        echo -e "\tOption      \"UseEDID\"           \"false\""
        echo -e "\tOption      \"UseEDIDDpi\"        \"false\""
        echo -e "\tOption      \"ModeValidation\"    \"NoEdidModes\""
    fi`
EndSection

Section "Screen"
	Identifier "Screen0"
	Device     "Card0"
	Monitor    "Monitor0"
	SubSection "Display"
		Viewport   0 0
		Depth     24
	EndSubSection
EndSection
EOF
}

function configure_x_service() {
    out "Configure X service ..."

    local x_service_file=${X_SERVICE_PATH:-$PWD}/X@.service

    debug "\tx_service_file: $x_service_file"

    cat > $x_service_file << EOF
[Unit]
Description=Xorg server DISPLAY=:%i

[Service]
ExecStart=/usr/bin/Xorg :%i -config ${X_CONFIG_PATH:-$PWD}/xorg-%i.conf -sharevts

[Install]
WantedBy=multi-user.target
EOF
}

function start_x_service() {
    out "Start X service..."

    for i in ${!X_DEV_BUS_ID[@]}
    do
        local x_id=`expr $i + 1`
        debug "\t==> start X@$x_id"
        systemctl start  X@$i && systemctl enable X@$x_id
        if [[ $? -ne 0 ]]; then
            warning "[Error] start X@$i service fail."
            return $FAILED
        fi
    done
    return 0
}

function stop_x_service() {
    out "Stop X service..."

    for i in ${!X_DEV_BUS_ID[@]}
    do
        local x_id=`expr $i + 1`
        debug "\t==> stop X@$x_id"
        systemctl stop  X@$i && systemctl disable X@$x_id
        if [[ $? -ne 0 ]]; then
            warning "[Error] stop X@$i service fail."
            return $FAILED
        fi
    done
    return 0
}


function clean_configure() {
    out "Clean configure ..."

    stop_x_service

    debug "\trm ${X_SERVICE_PATH:-$PWD}/X@.service"
    rm -rf ${X_SERVICE_PATH:-$PWD}/X@.service

    for i in ${!X_DEV_BUS_ID[@]}
    do
        local x_id=`expr $i + 1`
        debug "\trm ${X_CONFIG_PATH:-$PWD}/xorg-$x_id.conf"
        rm -rf ${X_CONFIG_PATH:-$PWD}/xorg-$x_id.conf
    done
}

function main() {
    out "Start deploy log:[$ALLLOG]"
    parse_args $@
    if [[ $? -ne 0 ]]; then
        return $FAILED
    fi
    out "Other arg(0x${DEBUG:-0}${QUIET:-0}${CLEAN:-0})"

    vga_devices_info
    if [[ $? -eq 0 ]]; then
        return $FAILED
    fi

    if [[ $CLEAN -eq 1 ]]; then
        clean_configure
        return 0
    fi

    out "Generate Xorg configure ..."
    for i in ${!X_DEV_BUS_ID[@]}
    do
        generate_xorg_conf $i $X_DEV_DRIVER ${X_DEV_BUS_ID[$i]}
    done

    configure_x_service
    start_x_service
    if [[ $? -ne 0 ]]; then
        return $FAILED
    fi

    out "Deploy success!!!"
    return 0
}

main $@
exit $?
