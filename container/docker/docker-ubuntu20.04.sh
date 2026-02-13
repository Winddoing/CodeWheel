#!/bin/bash
##########################################################
# Copyright (C) 2025  All rights reserved.
#  File Name    : docker-ubuntu20.04.sh
#  Author       : winddoing
#  Created Time : 2025-10-31 15:33:24
#  Description  :
##########################################################

set -x

docker ps -a | grep "ubuntu20.04" > /dev/null
if [ $? -ne 0 ]; then
	docker run -itd --name "ubuntu20.04" --hostname "ubuntu2004" -u $(id -u):$(id -g) -v /etc/passwd:/etc/passwd:ro -v /etc/group:/etc/group:ro -w $HOME -v $HOME:$HOME ubuntu:20.04
	docker exec -u 0 ubuntu20.04 apt update
	docker exec -u 0 ubuntu20.04 apt install -y tzdata
	docker exec -u 0 ubuntu20.04 ln -fs /usr/share/zoneinfo/Asia/Shanghai /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata
	docker exec -u 0 ubuntu20.04 apt install -y bash-completion vim git make gcc g++ expect gawk texinfo python libssl-dev bc liblz4-tool device-tree-compiler time rsync wget unzip cpio fakeroot
fi
#docker rm ubuntu20.04 -f

docker start ubuntu20.04
docker exec -it ubuntu20.04 bash
exit
