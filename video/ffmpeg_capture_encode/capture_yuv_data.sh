#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : capture_yuv_data.sh
#  Author       : wqshao
#  Created Time : 2023-01-13 11:39:14
#  Description  :
#    不同的分辨率需要，需要相应的桌面系统环境
##########################################################

for capture_resolution in 1920x1080 2560x1440 3840x2160
do
	for capture_pix_fmt in yuv444p yuv444p10le yuv420p yuv420p10le
	do
		set -x
		ffmpeg -video_size $capture_resolution -framerate 60 -f x11grab -i :0  -frames 200 -f rawvideo -pix_fmt $capture_pix_fmt test_data_$capture_resolution.$capture_pix_fmt
		set +x
	done #dec_chroma
done #dec_resolution
