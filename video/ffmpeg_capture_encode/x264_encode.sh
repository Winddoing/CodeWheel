#!/bin/bash
set -x

#Constraint baseline

#./x264 --profile baseline -B 6000000 --fps 30.0 --seek 20 --frames 5 --level 40 -o 720p/test_baseline.264 --input-res 1280x720 ~/jd_yuv420p_720p.yuv 
./x264 --profile baseline -B 6000000 --fps 30.0  --seek 20 --frames 10 --level 40 -o 720p/test_baseline_l52.264 --input-res 1280x720 --level 52 ~/jd_yuv420p_720p.yuv

# main
# ./x264 --profile main -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_main.264 --input-res 1280x720 ~/jd_yuv420p_720p.yuv
./x264 --profile main -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_main_l52.264 --input-res 1280x720 --level 52 ~/jd_yuv420p_720p.yuv

# high
# ./x264 --profile high -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high.264 --input-res 1280x720 ~/jd_yuv420p_720p.yuv
./x264 --profile high -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high_l52.264 --input-res 1280x720 --level 52 ~/jd_yuv420p_720p.yuv

# high10
# ./x264 --profile high10 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high10.264 --input-res 1280x720  --input-depth 10 --output-depth 10 ~/jd_yuv420p10le_720p.yuv
./x264 --profile high10 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high10_l52.264 --input-res 1280x720  --input-depth 10 --output-depth 10 --level 52 ~/jd_yuv420p10le_720p.yuv

# high 4:4:4 predictive 8bit
# ./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444.264 --input-res 1280x720  --input-csp i444 --output-csp i444 ~/jd_yuv444p_720p.yuv 
./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444_l52.264 --input-res 1280x720  --input-csp i444 --output-csp i444 --level 52 ~/jd_yuv444p_720p.yuv 

# high 4:4:4 predictive 10bit
# ./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444_p10.264 --input-res 1280x720  --input-csp i444 --output-csp i444 --input-depth 10 --output-depth 10 ~/jd_yuv444p10le_720p.yuv
./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444_p10_l52.264 --input-res 1280x720  --input-csp i444 --output-csp i444 --input-depth 10 --output-depth 10 --level 52 ~/jd_yuv444p10le_720p.yuv

# high 4:4:4 intra 8bit
# ./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444_intra.264 --input-res 1280x720  --input-csp i444 --output-csp i444 --keyint 1 ~/jd_yuv444p_720p.yuv 
./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444_intra_l52.264 --input-res 1280x720  --input-csp i444 --output-csp i444 --keyint 1 --level 52 ~/jd_yuv444p_720p.yuv 

# high 4:4:4 intra 10bit
# ./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444_intra_p10.264 --input-res 1280x720  --input-csp i444 --output-csp i444 --input-depth 10 --output-depth 10 --keyint 1 ~/jd_yuv444p10le_720p.yuv
./x264 --profile high444 -B 6000000 --fps 30.0 --seek 20 --frames 10 --level 40 -o 720p/test_high444_intra_p10_l52.264 --input-res 1280x720  --input-csp i444 --output-csp i444 --input-depth 10 --output-depth 10 --keyint 1 --level 52 ~/jd_yuv444p10le_720p.yuv