#!/bin/bash

#挂载configfs文件
mount -t configfs none /sys/kernel/config
cd /sys/kernel/config/usb_gadget
mkdir gadget
cd gadget
#ls一下，含有如下文
#// UDC bMaxPacketSize0 functions strings
#// bDeviceClass bcdDevice idProduct
#// bDeviceProtocol bcdUSB idVendor
#// bDeviceSubClass configs os_desc

#设置USB协议
echo 0x0200 > bcdUSB

#定义产品的VendorI
echo "0x0525" > idVendor
echo "0xa4a5" > idProduct

mkdir strings/0x409
ls strings/0x409

#//manufacturer product serialnumber
#将开发商、产品和序列号字
echo "01234567" > strings/0x409/serialnumber
echo "mkelehk" > strings/0x409/manufacturer
echo "upan" > strings/0x409/product

mkdir configs/config.1
ls configs/config.1

#//MaxPower bmAttributes strings

echo 120 > configs/config.1/MaxPower

#定义配置描述符使用的字
mkdir configs/config.1/strings/0x409
ls configs/config.1/strings/0x409/

#//configuration

echo "mass_storage" > configs/config.1/strings/0x409/configuration

#创建一个功能实例，需要注意的是，一个功能如果有多个实例的话，扩展名必须用数字编号：
mkdir functions/mass_storage.0

#创建一个功能实例，需要注意的是，一个功能如果有多
#配置U盘参数
#echo "/tmp/sdcard/disk.img" > functions/mass_storage.0/lun.0/file
echo "/dev/mmcblk0p3" > functions/mass_storage.0/lun.0/file
echo 1 > functions/mass_storage.0/lun.0/removable
echo 0 > functions/mass_storage.0/lun.0/nofua

#捆绑功能实例到配置config.1
ln -s functions/mass_storage.0 configs/config.1


ls /sys/class/udc/

echo "fe800000.usb_otg" > UDC