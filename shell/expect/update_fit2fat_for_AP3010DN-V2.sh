##########################################################
# File Name		: a.sh
# Author		: winddoing
# Created Time	: 2019年04月12日 星期五 17时12分45秒
# Description	:
##########################################################
#!/bin/bash

netstat=$(ping -c1 169.254.1.1 |grep transmitted |awk '{print $4}')
if [ "$netstat" -eq "0" ]; then
	echo " ping 169.254.1.1 fail..."
    echo "      ***********************************"
    echo "      * Network connection disconnected *"
    echo "      ***********************************"
    echo "Configuring a local network, IP: 169.254.1.100"
    exit 1
fi

ssh-keygen -f "$HOME/.ssh/known_hosts" -R "169.254.1.1"
echo "ssh-keygen -f "$HOME/.ssh/known_hosts" -R "169.254.1.1""

cat > update.exp << EOF
#!/usr/bin/expect -f

#set timeout 10
spawn ssh admin@169.254.1.1

expect {
	"*yes/no" {
		send "yes\r";
		exp_continue
	}
	"*password:" {
		send "admin@huawei.com\r"
	}
}

expect {
   "<Huawei>" {
       send "system-view\r"
       send "ap-mode-switch prepare\r"
       send "ap-mode-switch check\r"
	   set timeout -1
       send "ap-mode-switch tftp FatAP3010DN-V2_V200R008C10SPC500.bin 169.254.1.100\r"
   }
}

expect {
   "Y/N" {
	   set timeout 5
       send "Y\r\n"
   }
}

expect eof
EOF

# 命令执行存在超时时间，默认的超时时间是10s, 如果设置-1表示永不超时

chmod a+x update.exp
./update.exp
rm ./update.exp

echo ""
echo -n "Wait device reboot ..."
# ping fail
netstat=$(ping -c1 169.254.1.1 |grep transmitted |awk '{print $4}')
while [ "$netstat" -eq "0" ]
do
    echo -n "."
    netstat=$(ping -c1 169.254.1.1 |grep transmitted |awk '{print $4}')
done

echo ""
echo ""
echo "Reboot and Firmware upgrade:"

# ping success
netstat=$(ping -c2 169.254.1.1 |grep transmitted |awk '{print $4}')
while [ "$netstat" -ne "0" ]
do
    echo -n "#"
    netstat=$(ping -c2 169.254.1.1 |grep transmitted |awk '{print $4}')
done
echo ""
echo "Firmware upgrade succeeded"

echo ""
echo "Device restart:"

# ping fail
netstat=$(ping -c1 169.254.1.1 |grep transmitted |awk '{print $4}')
while [ "$netstat" -eq "0" ]
do
    echo -n "*"
    netstat=$(ping -c1 169.254.1.1 |grep transmitted |awk '{print $4}')
done
echo ""
echo "Device restart succeeded"

echo ""
echo "####################################################"
echo -e "#          \033[32m FIT to FAT update successed \033[0m           #"
echo "####################################################"
