#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : x265_encode.sh
#  Author       : wqshao
#  Created Time : 2023-01-13 14:17:34
#  Description  :
##########################################################

set -x

# 使用默认参数生成的码流，其中包含了B帧
#./build/linux/x265 --input-res 1920x1080 --fps 30 --input ~/Videos/ufotest_1920x1080.yuv420 --output ufotest_1920x1080_yuv420.265

#./build/linux/x265 --input-res 1920x1080 --fps 30 --input-depth 8 --input-csp i420 --input ~/Videos/ufotest_1920x1080.yuv420 --profile main --level-idc 51 --slices 4 --output-depth 8 --bitrate 8192 --bframes 0 --output ufotest_4slice_1920x1080_yuv420.265

# 码流结构与测试程序S端所录码流一致
#./build/linux/x265 --input-res 1920x1080 --fps 30 --input-depth 8 --input-csp i420 --input ~/Videos/ufotest_1920x1080.yuv420 --profile main --level-idc 51 --slices 4 --output-depth 8 --bitrate 8192 --bframes 0 --aud --output ufotest_4slice_aud_1920x1080_yuv420.265
#./build/linux/x265 --input-res 1920x1080 --fps 30 --input-depth 8 --input-csp i420 --input ~/Videos/ufotest_1920x1080.yuv420 --profile main --level-idc 51 --slices 1 --output-depth 8 --bitrate 8192 --bframes 0 --aud --output ufotest_1slice_aud_1920x1080_yuv420.265

# yuv444序列
#./build/linux/x265 --input-res 1920x1080 --fps 30 --input-depth 8 --input-csp i444 --input ~/Videos/ufotest_1920x1080.yuv444 --profile main444-8 --level-idc 51 --slices 4 --output-depth 8 --bitrate 8192 --bframes 0 --aud --output ufotest_4slice_aud_1920x1080_yuv444.265

# 测试IDR帧编码参数,每10帧添加一个I帧
#./build/linux/x265 --input-res 1920x1080 --fps 30 --input-depth 8 --input-csp i444 --input ~/Videos/ufotest_1920x1080.yuv444 --profile main444-8 --level-idc 51 --slices 4 --output-depth 8 --bitrate 8192 --bframes 0 --aud --keyint 10 --output ufotest_4slice_aud_1920x1080_yuv444_idr_cra.265
#./build/linux/x265 --input-res 1920x1080 --fps 30 --input-depth 8 --input-csp i444 --input ~/Videos/ufotest_1920x1080.yuv444 --profile main444-8 --level-idc 51 --slices 4 --output-depth 8 --bitrate 8192 --bframes 0 --aud --keyint 10 --no-open-gop --output ufotest_4slice_aud_1920x1080_yuv444_idr_n_lp.265
#./build/linux/x265 --input-res 1920x1080 --fps 30 --input-depth 8 --input-csp i444 --input ~/Videos/ufotest_1920x1080.yuv444 --profile main444-8 --level-idc 51 --slices 4 --output-depth 8 --bitrate 8192 --bframes 1 --aud --keyint 10 --no-open-gop --radl 1 --output ufotest_4slice_aud_1920x1080_yuv444_idr_w_radl.265

# yuv444 10bit
./build/linux/x265 --input-res 1920x1080 --fps 60 --input-depth 10 --input-csp i444 --input ~/Videos/ufotest_1920x1080.yuv444p10le --profile main444-10 --level-idc 51 --slices 4 --output-depth 10 --bitrate 8192 --bframes 0 --aud --output ufotest_4slice_1920x1080_yuv444p10.265
./build/linux/x265 --input-res 1920x1080 --fps 60 --input-depth 10 --input-csp i444 --input ~/Videos/ufotest_1920x1080.yuv444p10le --profile main444-10 --level-idc 51 --slices 1 --output-depth 10 --bitrate 8192 --bframes 0 --aud --output ufotest_1slice_1920x1080_yuv444p10.265

set +x
