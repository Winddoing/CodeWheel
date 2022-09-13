#/bin/bash
#set -x

PCI_ADDR="18:00.1"

modprobe vfio
modprobe vfio-pci
lsmod | grep vfio

lspci -s $PCI_ADDR -n  #em2

device_id=`lspci -s $PCI_ADDR -n | awk '{print $3}'`
device_id=${device_id/:/ } #去除：号
echo "PCI: $PCI_ADDR, Device ID:$device_id"

echo "$device_id" > /sys/bus/pci/drivers/vfio-pci/new_id
echo "$PCI_ADDR"  > /sys/bus/pci/drivers/vfio-pci/bind

pci_device=/sys/bus/pci/devices/0000:$PCI_ADDR/iommu_group/devices/
pci_device=`echo $pci_device | sed 's/:/\\:/g'` #添加转移符，echo打印不出来
#ls $pci_device
for dev in `ls $pci_device`
do
    echo "---dev:$dev"
    _pci_dev_unbind="/sys/bus/pci/devices/$dev/driver/unbind"
    _pci_dev_unbind=`echo $_pci_dev_unbind | sed 's/:/\\:/g'`
    #ls $_pci_dev_unbind
    echo "$dev" > $_pci_dev_unbind
    echo "$dev" > /sys/bus/pci/drivers/vfio-pci/bind
    lspci -s $dev -k
done

ls /dev/vfio/


#lspci -s 0000:18:00.1 -n  #em2
#echo "14e4 165f" > /sys/bus/pci/drivers/vfio-pci/new_id
#echo 0000:18:00.1 > /sys/bus/pci/devices/0000\:18\:00.1/driver/unbind
#echo 0000:18:00.1 > /sys/bus/pci/drivers/vfio-pci/bind 
#echo 0000:18:00.0 > /sys/bus/pci/devices/0000\:18\:00.0/driver/unbind
#echo 0000:18:00.0 > /sys/bus/pci/drivers/vfio-pci/bind 

# 编辑`/boot/efi/EFI/centos/grub.cfg`文件，在系统启动内核的选项`linuxefi`中追加`intel_iommu=on`

# https://blog.csdn.net/gerrylee93/article/details/106477055
# https://www.cnblogs.com/xia-dong/p/11542771.html
