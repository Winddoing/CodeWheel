#!/bin/bash

# high10
./x264 --profile high10 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 3840x2160 --fps 60 --cpu-independent --input-csp i420 --output-csp i420 --aud --bitrate 400000  --vbv-maxrate 400000 --vbv-bufsize 400000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/motor_4K60_yuv420p10le_400M.264 ~/motor_4K60_yuv420p10le.yuv
./x264 --profile high10 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 3840x2160 --fps 60 --cpu-independent --input-csp i420 --output-csp i420 --aud --bitrate 200000  --vbv-maxrate 200000 --vbv-bufsize 200000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/motor_4K60_yuv420p10le_200M.264 ~/motor_4K60_yuv420p10le.yuv

./x264 --profile high10 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 2560x1440 --fps 144 --cpu-independent --input-csp i420 --output-csp i420 --aud --bitrate 400000  --vbv-maxrate 400000 --vbv-bufsize 400000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/car_2K144_yuv420p10le_400M.264 ~/car_2K144_yuv420p10le.yuv
./x264 --profile high10 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 2560x1440 --fps 144 --cpu-independent --input-csp i420 --output-csp i420 --aud --bitrate 200000  --vbv-maxrate 200000 --vbv-bufsize 200000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/car_2K144_yuv420p10le_200M.264 ~/car_2K144_yuv420p10le.yuv
# high 4:4:4 predictive 10bit
./x264 --profile high444 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 3840x2160 --fps 60 --cpu-independent --input-csp i444 --output-csp i444 --aud --bitrate 400000  --vbv-maxrate 400000 --vbv-bufsize 400000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/motor_4K60_yuv444p10le_400M.264 ~/motor_4K60_yuv444p10le.yuv
./x264 --profile high444 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 3840x2160 --fps 60 --cpu-independent --input-csp i444 --output-csp i444 --aud --bitrate 200000  --vbv-maxrate 200000 --vbv-bufsize 200000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/motor_4K60_yuv444p10le_200M.264 ~/motor_4K60_yuv444p10le.yuv

./x264 --profile high444 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 2560x1440 --fps 144 --cpu-independent --input-csp i444 --output-csp i444 --aud --bitrate 400000  --vbv-maxrate 400000 --vbv-bufsize 400000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/car_2K144_yuv444p10le_400M.264 ~/car_2K144_yuv444p10le.yuv
./x264 --profile high444 --level 52 --bframes 0  --keyint=60  --input-depth 10 --output-depth 10  --input-res 2560x1440 --fps 144 --cpu-independent --input-csp i444 --output-csp i444 --aud --bitrate 200000  --vbv-maxrate 200000 --vbv-bufsize 200000 --slices 4 -o ~/ZC/tequila/CtrlSW/test264/car_2K144_yuv444p10le_200M.264 ~/car_2K144_yuv444p10le.yuv

