#!/bin/bash
PWD=`pwd`

os_id=$1

if [ "$#" -eq "0" ]; then
    os_id=0
fi
echo "virtual machine id=$os_id"

vcpu_nums=8
vmem_sz=8192

ANDROID_DIR="$PWD/android/A${os_id}"
echo "ANDROID_DIR=$ANDROID_DIR"

cpu_nums=`nproc`
s_cpu=$((vcpu_nums * os_id))
s_cpu=$((s_cpu%cpu_nums))
e_cpu=$(((1 + os_id) * vcpu_nums - 1))
e_cpu=$((e_cpu%cpu_nums))
echo "cpu_nums: $cpu_nums, virt cpu: $vcpu_nums, s_cpu:$s_cpu, e_cpu:$e_cpu"
if [ $((vcpu_nums*os_id)) -gt $cpu_nums ]; then
    echo "[Wall]: Virtual CPU configuration out of bounds！！！"
fi

free_mem_sz=`free -m | awk '/Mem/{printf "%d", $4}'`
echo "Sys free mem: $free_mem_sz MB, Virt mem: $vmem_sz MB"
if [ $vmem_sz -gt $free_mem_sz ]; then
    echo "[Error]: Configure memory overflow, reconfigure memory size ..."
fi

spice_port=$((3000+os_id))
adb_port=$((550+os_id))
echo "Spice port:[$spice_port], ADB port:[$adb_port]"

(gnome-terminal -- \
    taskset -c ${s_cpu}-${e_cpu} \
    qemu-system-aarch64 \
	-name android-$os_id \
	-m ${vmem_sz} -enable-kvm -machine virt,accel=kvm,gic-version=host -bios ${ANDROID_DIR}/QEMU_EFI.fd -cpu host -smp ${vcpu_nums}\
	-append "console=ttyAMA0,38400 earlycon=pl011,0x09000000 drm.debug=0x0 rootwait rootdelay=5 androidboot.selinux=permissive" \
	-kernel ${ANDROID_DIR}/Image \
	-initrd ${ANDROID_DIR}/ramdisk.img \
	-drive index=0,if=none,id=system,file=${ANDROID_DIR}/system.img \
	-device virtio-blk-pci,drive=system \
	-drive index=1,if=none,id=cache,file=${ANDROID_DIR}/cache.img \
	-device virtio-blk-pci,drive=cache \
	-drive index=2,if=none,id=userdata,file=${ANDROID_DIR}/userdata.img \
	-device virtio-blk-pci,drive=userdata \
	-netdev user,id=mynet,hostfwd=tcp::$adb_port-:5555 \
	-device virtio-net-pci,netdev=mynet \
	-device virtio-gpu-pci,virgl=on,edid=on,xres=1920,yres=1080 \
	-device virtio-serial-pci \
	-display sdl,gl=on \
	-spice port=$spice_port,disable-ticketing \
	-device nec-usb-xhci \
	-device virtio-keyboard-pci \
 	-device virtio-mouse-pci \
	-chardev spicevmc,name=usbredir,id=usbredirchardev1 \
	-device usb-redir,chardev=usbredirchardev1,id=usbredirdev1 \
	-chardev spicevmc,name=usbredir,id=usbredirchardev2 \
	-device usb-redir,chardev=usbredirchardev2,id=usbredirdev2 \
	-chardev spicevmc,name=usbredir,id=usbredirchardev3 \
	-device usb-redir,chardev=usbredirchardev3,id=usbredirdev3 \
	-device virtio-serial -chardev spiceport,id=port,debug=0,name=port -device virtserialport,chardev=port,name=com.vanxum.spice.0 \
	-serial mon:stdio \
	--full-screen \
	-soundhw all
)
