#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : iperf3_multi_run.sh
#  Author       : wqshao
#  Created Time : 2025-09-16 16:53:11
#  Description  :
##########################################################

arg=$1
echo "arg: $arg"

TEST_IP="16.16.16."
TEST_IP_START=100
#TEST_IP_END=131
TEST_IP_END=103
TEST_PORT_START=10520

server()
{
	echo "----server:"

	for idx in `seq $TEST_IP_START $TEST_IP_END`
	do
		ipaddr=${TEST_IP}${idx}
		port=$((TEST_PORT_START + idx))
		echo "ipaddr: $ipaddr, port: $port"
		run_cmd_shell=$(tempfile -p $0)
		echo "Run cmd shell: $run_cmd_shell"
		cpu_num=72
		cpu_id=$((idx % cpu_num))
		echo "cpu_num=$cpu_num, cpid_id=$cpu_id"
		cat > $run_cmd_shell << EOF
#!/bin/bash

set -x
#ssh wqshao@192.168.101.10 -p 2222 "echo "$ipaddr:$port"; taskset -c $cpu_id iperf3 -s -B $ipaddr -p $port 2>&1; echo "over."; while true; do sleep 10; done"
ssh wqshao@192.168.101.10 -p 2222 "echo "$ipaddr:$port"; iperf3 -s -B $ipaddr -p $port 1>&2; echo "over."; while true; do sleep 10; done"
#ssh wqshao@192.168.109.250 "pwd; ls; echo "$ipaddr:$port"; sleep 10"
EOF
		gnome-terminal -t "win-$ipaddr" --window -e "bash ${run_cmd_shell}"
	done
}

client()
{
	echo "----client:"

	for idx in `seq $TEST_IP_START $TEST_IP_END`
	do
		ipaddr=${TEST_IP}${idx}
		port=$((TEST_PORT_START + idx))
		echo "ipaddr: $ipaddr, port: $port"
		run_cmd_shell=$(tempfile -p $0)
		echo "Run cmd shell: $run_cmd_shell"
		cat > $run_cmd_shell << EOF
#!/bin/bash

set -x
ping $ipaddr -c 1
#ssh root@192.168.100.242 "echo "$ipaddr:$port"; iperf3 -c $ipaddr -p $port -R -t 86400; while true; do sleep 10; done"
#ssh root@192.168.100.242 "echo "$ipaddr:$port"; iperf3 -c $ipaddr -p $port -R -b 800M -t 40; echo "over"; while true; do sleep 10; done"
#ssh root@192.168.100.242 "echo "$ipaddr:$port"; iperf3 -c $ipaddr -p $port -R -t 40; echo "over"; while true; do sleep 10; done"
ssh root@192.168.100.242 "echo "$ipaddr:$port"; taskset -c 1,6-13,15 iperf3 -c $ipaddr -p $port -R -b 1000M -t 40; echo "over"; while true; do sleep 10; done"
#ssh root@192.168.100.242 "echo "$ipaddr:$port"; taskset -c 1,6-13,15 iperf3 -c $ipaddr -p $port -R -b 1000M -t 40 -P 5; echo "over"; while true; do sleep 10; done"
EOF
		gnome-terminal -t "win-$ipaddr" --window -e "bash ${run_cmd_shell}"
	done
}


if [ x"$arg" = x"s" ]; then
	echo "-----> Run server:"
	server
elif [ x"$arg" = x"c" ]; then
	echo "-----> Run client:"
	client
else
	echo "$0 [s/c]"
fi

