#!/bin/bash
##########################################################
# File Name		: vnc.sh
# Author		: winddoing
# Created Time	: 2020年10月14日 星期三 14时41分17秒
# Description	:
##########################################################

# sudo apt install xfce4 xfce4-goodies gnome-icon-theme
# sudo apt install tightvncserver
#  vncpasswd   #设置密码
#  vncserver   #主程序命令
# cat ~/.vnc/xstartup
##!/bin/sh
#xrdb $HOME/.Xresources
#startxfce4 &

set -x

display=:22

vncserver -kill $display
vncserver -name wqshao -depth 24 -geometry 1600x900 $display


# vnc://<IP>:5922
# sudo apt install xtightvncviewer
# vncviewer <IP>:5922


# https://linuxconfig.org/vnc-server-on-ubuntu-20-04-focal-fossa-linux
# https://sysadminjournal.com/how-to-install-vnc-server-on-ubuntu-20-04/
