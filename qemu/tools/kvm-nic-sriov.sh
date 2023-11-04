#!/bin/bash

echo 1 > /sys/class/net/eno1/device/sriov_numvfs

ip link set dev eno1 vf 0 mac 28:c1:3c:8a:a5:00
