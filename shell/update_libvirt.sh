#!/bin/bash
##########################################################
# File Name		: update_libvirt.sh
# Author		: winddoing
# Created Time	: 2020年09月14日 星期一 16时33分46秒
# Description	: install libvirt5.0 for centos7
##########################################################

#url="http://mirror.centos.org/centos/7.8.2003/virt/x86_64/libvirt-latest/"
url="http://mirrors.huaweicloud.com/centos/7/virt/x86_64/libvirt-latest/"

curl $url > page.txt

grep "5.0.0-1.el7.x86_64" page.txt > fff.txt

while read line
do
    #截取href前多余字符串
    aaa=${line: 24}
    #截取title后多余字符串
    bbb=${aaa%title*}
    #设置为href变量
    export $bbb
    #去掉变量值两边的引号
    ccc=`echo $href | sed 's/\"//g'`

    echo $url$ccc
    wget -P rpms $url$ccc

done < fff.txt

rm page.txt fff.txt -rf

yum -y install ./rpms/*.rpm virt-manager.noarch

systemctl restart libvirtd.service
