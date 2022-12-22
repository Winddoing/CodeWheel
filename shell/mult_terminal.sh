#!/bin/bash
##########################################################
# Copyright (C) 2022 wqshao All rights reserved.
#  File Name    : mult_terminal.sh
#  Author       : wqshao
#  Created Time : 2022-12-14 11:46:09
#  Description  :
##########################################################

TST_SH=$PWD/.loop_tst.sh

gen_test_sh()
{
	echo > $TST_SH
	cat >> $TST_SH << EOF
#!/bin/bash

while [ 1 ]
do
	glmark2-drm --off-screen --winsys-options "drm-device=/dev/dri/card1"
	sleep 3
done
EOF
	chmod +x $TST_SH
}

#gen_test_sh

for i in `seq 2`
do
	echo "-----------$i"
	#gnome-terminal -t "win-$i" --window -e "bash $TST_SH"
	gnome-terminal -t "win-$i" --window -e "glmark2-drm --off-screen --run-forever --winsys-options \"drm-device=/dev/dri/card1\""
done
