#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : audio_i2s_clk_cfg.sh
#  Author       : wqshao
#  Created Time : 2025-05-28 15:57:18
#  Description  :
##########################################################

#SAMPLE_RATE=(8000 11025 16000 22050 32000 44100 48000 64000 88200 96000 176400 192000 384000)
SAMPLE_RATE=(44100 48000 96000 192000)
SAMPLE_BITS=(16 24 32)
SAMPLE_CHAN=(1 2 4 8)

I2S_MODE=(TDM_8CH I2S_8CH TDM_2CH I2S_2CH)

S_BCLK_LRCLK_RATIO=1
S_FS=512

O_BCLK=0
O_LRCLK=0
O_MCLK=0

PLL3_CLK_MHZ=1474.56
PLL4_CLK_MHZ=677.376

PLL3_MIN_CLK=9999999999999
PLL3_MAX_CLK=0
PLL4_MIN_CLK=9999999999999
PLL4_MAX_CLK=0

#计算最大公约数(GCD)
gcd() {
	local a=$1
	local b=$2
	while [ $b -ne 0 ]; do
		local temp=$b
		b=$((a % b))
		a=$temp
	done
	echo $a
}

#计算最小公倍数(LCM)
lcm() {
	local a=$1
	local b=$2
	local gcd_val=$(gcd $a $b)
	echo $(((a * b) / gcd_val ))
}

ASIC_CLK_INFO_EN=1
FPGA_CLK_INFO_EN=0

show_asic_clk_info()
{
	if [ $ASIC_CLK_INFO_EN -gt 0 ]; then
		if [ $ASIC_CLK_INFO_EN -eq 1 ]; then
			printf "%3s\t%s\t%7s\t%4s\t%4s\t%6s\t%5s\t%6s\t%s\t%9s\t%s\t%9s\t%s\t%s\t%s\t%s\t%s\n" \
				"id" "I2S CTL" "mode" "channel" "bits" "rate" "FS" "BCLK/LRCLK" "LRCLK" "BCLK" "BCLK(MHz)" "MCLK" "MCLK(MHz)" "PLL" "PLL(MHZ)" "MDIV" "SDIV"
		fi
		printf "[%03d]\t%s\t%7s\t%4d\t%4d\t%6d\t%5d\t%6d\t%12d\t%9d\t%9.2f\t%9d\t%9.2f\t%s\t%8.2f\t%.2f\t%.2f\n" \
			$CNT $I2S_CTL_NAME $mode $chan $bits $rate $S_FS $S_BCLK_LRCLK_RATIO $O_LRCLK $O_BCLK $O_BCLK_MHZ $O_MCLK $O_MCLK_MHZ $PLL_NAME $PLL_CLK_MHZ $MDIV $SDIV

		ASIC_CLK_INFO_EN=2
	fi
}

show_fpga_clk_info()
{
	if [ $FPGA_CLK_INFO_EN -gt 0 ]; then
		if [ $FPGA_CLK_INFO_EN -eq 1 ]; then
			printf "%3s\t%s\t%7s\t%4s\t%4s\t%6s\t%5s\t%6s\t%s\t%9s\t%s\t%9s\t%s\t%s\t%s\t%s\t%s\n" \
				"id" "I2S CTL" "mode" "channel" "bits" "rate" "FS" "BCLK/LRCLK" "LRCLK" "BCLK" "BCLK(MHz)" "MCLK" "MCLK(MHz)" "S2CCLK_4(BCLK)" "S2CCLK_6(MCLK)" "S2CCLK_3(BCLK)" "S2CCLK_5(MCLK)"
		fi
		printf "[%03d]\t%s\t%7s\t%4d\t%4d\t%6d\t%5d\t%6d\t%12d\t%9d\t%9.2f\t%9d\t%9.2f\t%14.3f\t%14.3f\t%14.3f\t%14.3f\n" \
			$CNT $I2S_CTL_NAME $mode $chan $bits $rate $S_FS $S_BCLK_LRCLK_RATIO $O_LRCLK $O_BCLK $O_BCLK_MHZ $O_MCLK $O_MCLK_MHZ $S2CCLK_4 $S2CCLK_6 $S2CCLK_3 $S2CCLK_5

		FPGA_CLK_INFO_EN=2
	fi

}


CNT=1
for mode in ${I2S_MODE[@]}; do
	for rate in ${SAMPLE_RATE[@]}; do
		for bits in ${SAMPLE_BITS[@]}; do
			for chan in ${SAMPLE_CHAN[@]}; do
				case $mode in
					TDM_8CH)
						# TDM256
						S_BCLK_LRCLK_RATIO=$((32 * 8))
						#[ $mode == TDM_8CH ] && [ $chan == 1 ] && continue;
						;;
					TDM_2CH)
						# TDM64
						S_BCLK_LRCLK_RATIO=$((32 * 2))
						#[ $mode == TDM_8CH ] && [ $chan == 1 ] && continue;
						;;
					I2S*)
						# I2S协议默认为左右立体声，因此即使8声道时计算计算BCLK也是按照左右两声道计算
						S_BCLK_LRCLK_RATIO=$((bits * 2))
						[ $chan == 1 ] && continue;
						;;
				esac

				O_LRCLK=$rate
				O_BCLK=$((O_LRCLK * S_BCLK_LRCLK_RATIO))
				O_MCLK=$((O_LRCLK * S_FS))

				O_BCLK_MHZ=$(echo "scale=4;$O_BCLK/1000000" | bc)
				O_MCLK_MHZ=$(echo "scale=4;$O_MCLK/1000000" | bc)

				#printf "[%03d]\t%7s\t%4d\t%4d\t%6d\t%5d\t%6d\t%12d\t%9d\t%9.2f\t%9d\t%.2f\n" $CNT $mode $chan $bits $rate $S_FS $S_BCLK_LRCLK_RATIO $O_LRCLK $O_BCLK $O_BCLK_MHZ $O_MCLK $O_MCLK_MHZ

				# 分频系数
				#        +----+     
				#      +-|SDIV|-BCLK
				#      | +----+     
				# PLL--+            
				#      | +----+     
				#      +-|MDIV|-MCLK
				#        +----+     
				case $mode in 
					TDM_8CH | I2S_8CH)
						# I2S0/1 IO复用可以共用PLL3
						PLL3_CLK=$(lcm $O_BCLK $O_MCLK)
						[ $PLL3_CLK -gt $PLL3_MAX_CLK ] && PLL3_MAX_CLK=$PLL3_CLK;
						[ $PLL3_CLK -lt $PLL3_MIN_CLK ] && PLL3_MIN_CLK=$PLL3_CLK;

						PLL3_CLK_MHZ=$(echo "scale=4;$PLL3_CLK / 1000000" | bc)
						PLL3_MAX_CLK_MHZ=$(echo "scale=4;$PLL3_MAX_CLK / 1000000" | bc)
						PLL3_MIN_CLK_MHZ=$(echo "scale=4;$PLL3_MIN_CLK / 1000000" | bc)
						#echo "-----PLL3_CLK=$PLL3_CLK, PLL3_MIN_CLK=$PLL3_MIN_CLK, PLL3_MIN_CLK_MHZ=$PLL3_MIN_CLK_MHZ"

						PLL3_MDIV=$(echo "scale=2;$PLL3_CLK / $O_MCLK" | bc)
						PLL3_SDIV=$(echo "scale=2;$PLL3_CLK / $O_BCLK" | bc)

						#echo "mode=$mode, FS=$S_FS, bits=$bits, LRCLK=$O_LRCLK, BCLK=$O_BCLK, MCLK=$O_MCLK, MCLK/BCLK=$(echo "scale=2;$O_MCLK/$O_BCLK" | bc), PLL3_CLK=$PLL3_CLK, PLL3_CLK_MHZ=$PLL3_CLK_MHZ"
						#printf "mode:%s FS:%d (%d/%d/%-6d)\tBCLK=%d\tMCLK=%d\tPLL3_CLK=%d\tPLL3_MDIV=%.2f\tPLL3_SDIV=%.2f\n" $mode $S_FS $chan $bits $O_LRCLK $O_BCLK $O_MCLK $PLL3_CLK $PLL3_MDIV $PLL3_SDIV

						I2S_CTL_NAME="I2S0/1"
						PLL_NAME="PLL3"
						PLL_CLK_MHZ=$PLL3_CLK_MHZ
						MDIV=$PLL3_MDIV
						SDIV=$PLL3_SDIV

						# for fpga
						S2CCLK_4=0
						S2CCLK_6=0
						S2CCLK_3=$O_BCLK_MHZ
						S2CCLK_5=$O_MCLK_MHZ
						;;
					TDM_2CH | I2S_2CH)
						# I2S2使用PLL4
						PLL4_CLK=$(lcm $O_BCLK $O_MCLK)
						[ $PLL4_CLK -gt $PLL4_MAX_CLK ] && PLL4_MAX_CLK=$PLL4_CLK;
						[ $PLL4_CLK -lt $PLL4_MIN_CLK ] && PLL4_MIN_CLK=$PLL4_CLK;

						PLL4_CLK_MHZ=$(echo "scale=4;$PLL4_CLK / 1000000" | bc)
						PLL4_MAX_CLK_MHZ=$(echo "scale=4;$PLL4_MAX_CLK / 1000000" | bc)
						PLL4_MIN_CLK_MHZ=$(echo "scale=4;$PLL4_MIN_CLK / 1000000" | bc)
						#echo "-----PLL4_CLK=$PLL4_CLK, PLL3_MIN_CLK=$PLL4_MIN_CLK, PLL3_MIN_CLK_MHZ=$PLL4_MIN_CLK_MHZ"

						PLL4_MDIV=$(echo "scale=2;$PLL4_CLK / $O_MCLK" | bc)
						PLL4_SDIV=$(echo "scale=2;$PLL4_CLK / $O_BCLK" | bc)

						#echo "mode=$mode, FS=$S_FS, bits=$bits, LRCLK=$O_LRCLK, BCLK=$O_BCLK, MCLK=$O_MCLK, MCLK/BCLK=$(echo "scale=2;$O_MCLK/$O_BCLK" | bc), PLL3_CLK=$PLL4_CLK, PLL4_CLK_MHZ=$PLL4_CLK_MHZ"
						#printf "mode:%s FS:%d (%d/%d/%-6d)\tBCLK=%d\tMCLK=%d\tPLL4_CLK=%d\tPLL4_MDIV=%.2f\tPLL3_SDIV=%.2f\n" $mode $S_FS $chan $bits $O_LRCLK $O_BCLK $O_MCLK $PLL4_CLK $PLL4_MDIV $PLL4_SDIV

						I2S_CTL_NAME="I2S2"
						PLL_NAME="PLL4"
						PLL_CLK_MHZ=$PLL4_CLK_MHZ
						MDIV=$PLL4_MDIV
						SDIV=$PLL4_SDIV

						# for fpga
						S2CCLK_4=$O_BCLK_MHZ
						S2CCLK_6=0
						S2CCLK_3=0
						S2CCLK_5=0
						;;
				esac

				if [ $FPGA_CLK_INFO_EN -ne 0 ]; then
					show_fpga_clk_info
				else
					show_asic_clk_info
				fi
				#printf "[%03d]\t%s\t%7s\t%4d\t%4d\t%6d\t%5d\t%6d\t%12d\t%9d\t%9.2f\t%9d\t%9.2f\t%s\t%8.2f\t%.2f\t%.2f\n" \
				#	$CNT $I2S_CTL_NAME $mode $chan $bits $rate $S_FS $S_BCLK_LRCLK_RATIO $O_LRCLK $O_BCLK $O_BCLK_MHZ $O_MCLK $O_MCLK_MHZ $PLL_NAME $PLL_CLK_MHZ $MDIV $SDIV

				if [ $O_BCLK -eq $O_MCLK ]; then
					echo "------> The clocks of BCLK($O_BCLK_MHZ) and MCLK($O_MCLK_MHZ) are the same, but the FS($S_FS) configuration is abnormal"
				fi

				CNT=$((CNT + 1))
			done
		done
	done
done

echo "Audio PLL: [PLL3 CLK(MAX/MIN) : $PLL3_MAX_CLK_MHZ/$PLL3_MIN_CLK_MHZ MHz, PLL4 CLK(MAX/MIN) : $PLL4_MAX_CLK_MHZ/$PLL4_MIN_CLK_MHZ MHz]"
