#!/bin/bash
##########################################################
# File Name		: boot.sh
# Author		: winddoing
# Created Time	: 2020年12月07日 星期一 11时38分25秒
# Description	:
##########################################################

sudo qemu-system-x86_64 \
    -M q35 -cpu host -smp 6,sockets=1,cores=6,threads=1 -m 16G -enable-kvm \
    -name ubuntu-20.04  \
    -drive file=$PWD/ubuntu20.04.qcow2,format=qcow2,if=virtio \
    -device qemu-xhci,id=usb \
    -device usb-kbd -device usb-mouse -device usb-tablet \
    -device virtio-gpu-pci,virgl=on,edid=on,xres=1920,yres=1080 \
    -vga none \
    -display sdl,gl=on \
    -full-screen

