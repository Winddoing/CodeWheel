#!/bin/bash
# Qemu 7.0.0
PWD=`pwd`

vm_id=$1

if [ "$#" -eq "0" ]; then
    vm_id=0
fi
echo "virtual machine id=$vm_id"

ANDROID_DIR="/home/vgtp-x/android/A${vm_id}"
echo "ANDROID_DIR=$ANDROID_DIR"

machine=`uname -m`

case "$machine" in
    arm64)
        vcpu_nums=8
        vmem_sz=8192
        QEMU_OPTS="-enable-kvm -machine virt,accel=kvm,gic-version=host -bios ${ANDROID_DIR}/QEMU_EFI.fd"
        KERNEL_CMDLINE='console=ttyAMA0,38400 earlycon=pl011,0x09000000 drm.debug=0x0 rootwait rootdelay=5 androidboot.selinux=permissive'
        KERNEL=${ANDROID_DIR}/Image
        ;;
    x86_64)
        vcpu_nums=2
        vmem_sz=4096
        QEMU_OPTS="-enable-kvm"
        KERNEL_CMDLINE='console=tty0 console=ttyS0 debug drm.debug=0x0 androidboot.selinux=permissive androidboot.hardware=vanxum'
        KERNEL=${ANDROID_DIR}/bzImage
        ;;
esac

spice_port=$((3000+vm_id))
adb_port=$((5000+vm_id))
echo "Spice port:[$spice_port], ADB port:[$adb_port]"

#(gnome-terminal -- \
qemu-system-$machine \
    -name android-$vm_id \
    -m ${vmem_sz} -smp ${vcpu_nums} -cpu host ${QEMU_OPTS} \
    -append "${KERNEL_CMDLINE}" \
    -kernel ${KERNEL} \
    -initrd ${ANDROID_DIR}/ramdisk.img \
    -drive index=0,if=none,id=system,file=${ANDROID_DIR}/system.img \
    -device virtio-blk-pci,drive=system \
    -drive index=1,if=none,id=cache,file=${ANDROID_DIR}/cache.img \
    -device virtio-blk-pci,drive=cache \
    -drive index=2,if=none,id=userdata,file=${ANDROID_DIR}/userdata.img \
    -device virtio-blk-pci,drive=userdata \
    -netdev user,id=mynet,hostfwd=tcp::$adb_port-:5555 \
    -device virtio-net-pci,netdev=mynet \
    -device virtio-gpu-gl-pci,edid=on,xres=1920,yres=1080 \
    -vga none \
    -display sdl,gl=on \
    -device virtio-serial-pci \
    -device nec-usb-xhci \
    -device virtio-keyboard-pci \
    -device virtio-mouse-pci \
    -device ich9-usb-ehci1,id=usb \
    -device ich9-usb-uhci1,masterbus=usb.0,firstport=0,multifunction=on \
    -device ich9-usb-uhci2,masterbus=usb.0,firstport=2 \
    -device ich9-usb-uhci3,masterbus=usb.0,firstport=4 \
    -chardev spicevmc,name=usbredir,id=usbredirchardev1 \
    -device usb-redir,chardev=usbredirchardev1,id=usbredirdev1 \
    -chardev spicevmc,name=usbredir,id=usbredirchardev2 \
    -device usb-redir,chardev=usbredirchardev2,id=usbredirdev2 \
    -chardev spicevmc,name=usbredir,id=usbredirchardev3 \
    -device usb-redir,chardev=usbredirchardev3,id=usbredirdev3 \
    --full-screen \
    -d guest_errors -nodefaults -serial mon:stdio
    #)
