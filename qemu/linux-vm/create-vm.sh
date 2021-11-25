#!/bin/bash
##########################################################
# File Name		: creat_vm.sh
# Author		: winddoing
# Created Time	: 2020年12月07日 星期一 11时04分26秒
# Description	:
##########################################################

set -x

qemu-system-x86_64 \
    -m 8G -enable-kvm -M pc -cpu host -smp 4 \
    -drive file=$PWD/ubuntu20.04.qcow2,if=virtio \
    -cdrom ubuntu-20.04.1-desktop-amd64.iso \
    -boot cd \
    -display gtk

