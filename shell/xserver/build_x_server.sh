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

function generate_edid() {
    out "Generate_edid ..."

    X_DEV_EDID="${X_CONFIG_PATH:-$PWD}/edid.bin"
    debug "\tX_DEV_EDID=$X_DEV_EDID"

    sed -n '1,/^exit $?$/!p' $0 > $X_DEV_EDID
}

function generate_xorg_conf() {
    local x_conf_id=`expr $1 + 1`
    local device_driver=$2
    local device_busid="PCI:$3"
    local device_edid=$4
    local xorg_conf_file="${X_CONFIG_PATH:-$PWD}/xorg-$x_conf_id.conf"

    debug "\tgenerate_xorg_conf: x_conf_id=$x_conf_id"
    debug "\tdevice_driver=$device_driver, device_busid=$device_busid"
    debug "\txorg_conf_file=$xorg_conf_file"
    cat > $xorg_conf_file << EOF
Section "ServerLayout"
    Identifier     "X.org Configured"
    Screen      0  "Screen0" 0 0
    InputDevice    "Mouse0" "CorePointer"
    InputDevice    "Keyboard0" "CoreKeyboard"
EndSection

Section "InputDevice"
    Identifier  "Keyboard0"
    Driver      "kbd"
EndSection

Section "InputDevice"
    Identifier  "Mouse0"
    Driver      "mouse"
    Option	    "Protocol" "auto"
    Option	    "Device" "/dev/input/mice"
    Option	    "ZAxisMapping" "4 5 6 7"
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

    Option      "ProbeAllGpus" "false"
    Option      "ConnectedMonitor" "DFP-0"
    Option      "CustomEDID" "DFP-0:$device_edid"
    Option      "IgnoreEDID" "false"
    Option      "UseEDID" "ture"
EndSection

Section "Screen"
    Identifier "Screen0"
    Device     "Card0"
    Monitor    "Monitor0"
    SubSection "Display"
        Viewport   0 0
        Depth     1
    EndSubSection
    SubSection "Display"
        Viewport   0 0
        Depth     4
    EndSubSection
    SubSection "Display"
        Viewport   0 0
        Depth     8
    EndSubSection
    SubSection "Display"
        Viewport   0 0
        Depth     15
    EndSubSection
    SubSection "Display"
        Viewport   0 0
        Depth     16
    EndSubSection
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
    out "Start X service"

    for i in ${!X_DEV_BUS_ID[@]}
    do
        debug "\t==> start X@$i"
        systemctl start  X@$i && systemctl enable X@$i
        if [[ $? -ne 0 ]]; then
            return $FAILED
        fi
    done
    return 0
}

function main() {
    out "Start deploy log:[$ALLLOG]"
    parse_args $@
    if [[ $? -ne 0 ]]; then
        return $FAILED
    fi
    out "Other arg(0x${DEBUG:-0}${QUIET:-0})"

    vga_devices_info
    if [[ $? -eq 0 ]]; then
        return $FAILED
    fi

    generate_edid

    out "Generate Xorg configure ..."
    for i in ${!X_DEV_BUS_ID[@]}
    do
        generate_xorg_conf $i $X_DEV_DRIVER ${X_DEV_BUS_ID[$i]} $X_DEV_EDID
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
