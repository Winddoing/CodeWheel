#!/bin/bash
##########################################################
# File Name		: audio.sh
# Author		: winddoing
# Created Time	: 2022年02月23日 星期三 10时33分37秒
# Description	:
##########################################################

#i2s0_mclk=audiopll_clk/(i2s0_mclk_div+1)
#i2s0_sclk=i2s0_mclk/(i2s0_sclk_div+1)

audiopll_clk=$((1536*1000*1000))
#audiopll_clk=$((1225*1000*1000))

#mclk_fs_32=128 256 512
#mclk_fs_24=192 384 768

# 24,32,2三者的最大公约数，目的是为了在分频时尽可能除尽
mclk_fs=384

ROUNDING=1  #分频系数进行四舍五入

#可用的最大分频系数
sclk_div_max=0x7F
mclk_div_max=0xFFF
sclk_div_max=$(printf "%d\n" $sclk_div_max)
mclk_div_max=$(printf "%d\n" $mclk_div_max)
echo "mclk_div_max=$mclk_div_max, sclk_div_max=$sclk_div_max"

i=0
for rate in 8000 11025 16000 22050 32000 44100 48000 64000 88200 96000 176400 192000
do
	for bit in 16 24 32
	do
		for channel in 2
		do
			i=$((i+1))
			#[ $bit -eq 24 ] && mclk_fs=$mclk_fs_24
			printf "[%02d]apll=%d rate=%-6d bit=%d ch=%d fs=%d\t" $i $audiopll_clk $rate $bit $channel $mclk_fs

			mclk=$((rate * mclk_fs))
			if [ $ROUNDING -ne 1 ]; then
				mclk_div=$((audiopll_clk / mclk - 1))
			else
				mclk_div=$(echo "scale=4; $audiopll_clk/$mclk" - 1 | bc)
			fi
			printf "mclk=%-8d mclk_div=%-3.2f\t" $mclk $mclk_div

			sclk=$((rate * bit * channel))
			if [ $ROUNDING -ne 1 ]; then
				sclk_div=$((mclk / sclk - 1))
			else
				sclk_div=$(echo "scale=4;$mclk/$sclk" - 1 | bc)
			fi
			printf "sclk=%-8d sclk_dev=%-3.2f\t" $sclk $sclk_div

			if [ $ROUNDING -eq 1 ]; then
				mclk_div=$(printf "%0.f" $mclk_div)
				sclk_div=$(printf "%0.f" $sclk_div)
			fi
			printf "[mclk_div=%-3d(%-3d) sclk_div=%-2d(%-2d)]\t" $mclk_div $mclk_div_max $sclk_div $sclk_div_max

			apll_div=$(((mclk_div + 1) * (sclk_div + 1)))
			bclk=$(echo "scale=0; $audiopll_clk/$apll_div" | bc)

			printf "bclk=%-8d\t" $bclk

			# 计算误差
			if [ $bclk -lt $sclk ]; then
				#小于
				diff=$(echo "scale=4; ($sclk-$bclk)/$sclk * 100" | bc)
				printf "diff=-%0.2f" $diff
			else
				diff=$(echo "scale=4; ($bclk-$sclk)/$sclk * 100" | bc)
				printf "diff=+%0.2f" $diff
			fi
			printf "\n"
		done
	done
done

