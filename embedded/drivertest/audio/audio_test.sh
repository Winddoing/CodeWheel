#!/bin/bash
##########################################################
# File Name		: audio/audio_test.sh
# Author		: winddoing
# Created Time	: 2021年11月24日 星期三 11时30分42秒
# Description	:
##########################################################

PWD=$(pwd)
TST_DIR="$PWD/stree_audio"
SYS_TST_FILE=/media/eee.wav
LOOP_TST_RP=0
pcnt=1
rcnt=1

okay() {
    echo -e "\e[32m$@\e[0m"
}

fail() {
    echo -e "\e[01;31m$@\e[0m"
}

notify() {
    echo -e "\e[33m$@\e[0m"
}


record_play_test()
{
	local card_id=$1
	local p_tst_dir=$2
	local r_tst_dir=$3
	local play_clean=$4

	echo "[$card_id] --> Record start"
	for fmt in S16_LE S24_LE S32_LE
	do
		for rate in 8000 16000 32000 44100 48000 96000
		do
			rec_file="audio_record_2c_${fmt}_${rate}_$RANDOM.wav"
			echo "[$card_id] -->Loop($rcnt) record, audio file: $rec_file, fmt: $fmt, rate: $rate"
			set -x
			arecord -D plughw:$card_id,0 -d 10 -r $rate -f $fmt -c 2 -t wav $r_tst_dir/$rec_file
			set +x
			rcnt=$((rcnt + 1))
			sleep 1
		done
	done
	echo "[$card_id] -->Record over"
	sleep 1

	for snd_file in `ls $r_tst_dir/*.wav`
	do
		echo "[$card_id] Loop($pcnt) play recording files, audio file: $snd_file"
		set -x
		aplay -D plughw:$card_id,0 $snd_file
		set +x
		pcnt=$((pcnt + 1))
		if [ x"$play_clean" == x"1" ]; then echo "[$card_id] Del record file $snd_file"; rm $snd_file -rf; fi
		sleep 1
	done
}

loop_play_or_record_test()
{
	local card_id=$1
	local p_tst_dir="$TST_DIR/$card_id/p"
	local r_tst_dir="$TST_DIR/$card_id/r"

	echo "[$card_id] *** Test sound card: `cat /sys/class/sound/card$card_id/device/modalias`"

	echo "[$card_id] *** Prepare audio files PWD=$PWD ***"
	if [ ! -d $p_tst_dir ]; then
		mkdir -p $p_tst_dir
	fi
	if [ ! -d $r_tst_dir ]; then
		mkdir -p $r_tst_dir
	fi
	rm $p_tst_dir/* $r_tst_dir/* -rf

	cp $SYS_TST_FILE $p_tst_dir -arpv
	cp $PWD/*.wav $p_tst_dir -arpv
	sync

	echo "[$card_id] ########## Cycle stress test Play/Record card_id=$card_id ##########"
	echo "p_tst_dir:$p_tst_dir, r_tst_dir:$r_tst_dir"
	while [ 1 ];
	do
		for snd_file in `ls $p_tst_dir`
		do
			echo "[$card_id] Loop($pcnt) play local files, audio file: $snd_file"
			set -x
			aplay -D plughw:$card_id,0 $p_tst_dir/$snd_file
			set +x
			pcnt=$((pcnt + 1))
			sleep $card_id
		done

		[ $LOOP_TST_RP == 1 ] && record_play_test $card_id $p_tst_dir $r_tst_dir 1
	done
	echo "[$card_id] ########### Cycle stress test over ##########"
}

loop_stree_test()
{
	local tst_card_id=$1

	for id in $(seq 0 5)
	do
		modalias="/sys/class/sound/card${id}/device/modalias"
		[ -f $modalias ] || continue

		if [ $tst_card_id ]; then
			LOOP_TST_RP=1
			[ $tst_card_id == $id ] || continue;
		fi

		echo "id=$id, modalias=$(cat $modalias)"

		grep "rt5651" $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &
		grep "es8311" $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &
		grep "es8326" $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &
		grep "rt5616" $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &
		grep "rt5640" $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &
		grep "usb"  $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &  #USB codec

		grep "hdmi-sound0" $modalias 2>&1 >/dev/null && LOOP_TST_RP=0 loop_play_or_record_test $id &
		grep "hdmi-sound1" $modalias 2>&1 >/dev/null && LOOP_TST_RP=0 loop_play_or_record_test $id &

		sleep 1
		echo ""
	done

	wait

	return 0
}

loopback_test()
{
	local tst_file=$1

	echo "------ Loopback Test ------"
	for id in $(seq 0 5)
	do
		modalias="/sys/class/sound/card${id}/device/modalias"
		[ -f $modalias ] || continue

		echo "id=$id, modalias=$(cat $modalias)"

		grep "hdmi" $modalias 2>&1 >/dev/null && while [ 1 ]; do aplay -D plughw:$id,0 ${tst_file:-$SYS_TST_FILE}; done &
		grep "hdmi" $modalias 2>&1 >/dev/null || arecord -D plughw:$id,0 -r 48000 -f S16_LE -c 2 -t wav | aplay -D plughw:$id,0 &

		sleep 0.5
		echo ""
	done

	wait

	return 0
}

es8326_ext_loopback_test_start_config()
{
	local id=$1
	echo "$FUNCNAME:"

	set -x
	amixer -c $id cset numid=11,iface=MIXER,name='ALC Capture Switch' 0
	amixer -c $id cset numid=1,iface=MIXER,name='DAC Playback Volume' 148
	set +x
}

es8326_ext_loopback_test_end_config()
{
	local id=$1
	echo "$FUNCNAME:"

	set -x
	amixer -c $id cset numid=11,iface=MIXER,name='ALC Capture Switch' 1
	amixer -c $id cset numid=1,iface=MIXER,name='DAC Playback Volume' 170
	set +x
}

spv4040_ext_loopback_test_start_config()
{
	local id=$1
	echo "$FUNCNAME:"

	set -x
	amixer -c $id cset numid=6,iface=MIXER,name='PCM Playback Volume' 87
	set +x

}

spv4040_ext_loopback_test_end_config()
{
	local id=$1
	echo "$FUNCNAME:"

	set -x
	amixer -c $id cset numid=6,iface=MIXER,name='PCM Playback Volume' 120
	set +x

}

# 工具fpcalc
# - sudo apt install libchromaprint-tools
# - git clone https://github.com/acoustid/chromaprint.git
comparison_audio_fingerprints()
{
	local src_audio_file=$1
	local dts_audio_file=$2
	local no_hp_audio_fp="AQAAOUmUaEkSRZEGAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

	echo "src file:[$src_audio_file], dts file:[$dts_audio_file]"

	which fpcalc > /dev/null
	if [ $? -ne 0 ]; then
		fail "No fpcalc command found."
		return 111
	fi

	local src_file_fp=`fpcalc -plain $src_audio_file`
	local dts_file_fp=`fpcalc -plain $dts_audio_file`

	echo ""
	echo " src fingerprint: [$src_file_fp]"
	echo " dts fingerprint: [$dts_file_fp]"
	echo "  no hp audio fp: [$no_hp_audio_fp] "
	echo ""

	d=$(diff <(echo "$src_file_fp" |sed 's/./&\n/g') <(echo "$dts_file_fp" |sed 's/./&\n/g') | grep -c '^[-+]')
	sim=$((100-100*d/(${#src_file_fp}+${#dts_file_fp})))
	notify "Test playback recording audio file fingerprint similarity: [$sim%]"
	echo ""

	if [ x"$src_file_fp" = x"$dts_file_fp" ]; then
		echo "Source and target fingerprints are a match."
		return 0
	fi

	if [ x"$dts_file_fp" = x"$no_hp_audio_fp" ]; then
		fail "Source and target fingerprints do not match, and the headset is not inserted without MIC."
		return 1
	fi

	fail "Source and target fingerprints do not match."
	return 255
}

ext_loopback_test()
{
	local card_id=$1

	card_id=${card_id:-0}

	echo "------Ext Loopback Test card_id=$card_id ------"

	if [ ! -f "/proc/asound/card$card_id/pcm0c/info" ]; then
		fail "$FUNCNAME, no support recording."
		return -1
	fi

	grep "es8326" /proc/asound/card$card_id/id 2>&1 >/dev/null && es8326_ext_loopback_test_start_config $card_id
	grep "Audio" /proc/asound/card$card_id/id 2>&1 >/dev/null && spv4040_ext_loopback_test_start_config $card_id

	local player_audio_file="/media/1kHz_sine_16bit_48kHz.wav"
	local record_audio_file="/tmp/ext_record_1kHz_sine_16bit_48kHz.wav"

	set -x
	aplay -D hw:$card_id,0 -d 10 $player_audio_file &
	sleep 0.1
	arecord -D hw:$card_id,0 -c 2 -r 48000 -f s16_le -t wav -d 10 $record_audio_file &
	sleep 0.2
	set +x

	echo ""
	while [ 1 ]
	do
		monitor=`ps -ef | grep "1kHz_sine_16bit_48kHz" | grep -v grep | wc -l`
		if [ $monitor -eq 0 ]; then
			echo -e "\n"
			break
		fi
		sleep 0.1
		echo -n "#"
	done

	grep "es8326" /proc/asound/card$card_id/id 2>&1 >/dev/null && es8326_ext_loopback_test_end_config $card_id
	grep "Audio" /proc/asound/card$card_id/id 2>&1 >/dev/null && spv4040_ext_loopback_test_end_config $card_id


	#对比音频指纹
	comparison_audio_fingerprints $player_audio_file $record_audio_file
	if [ $? -eq 0 ]; then
		echo ""
		okay "*********************************************************************************"
		okay "*                                                                               *"
		okay "*           audio playback and recording functions are normal                   *"
		okay "*                                                                               *"
		okay "*********************************************************************************"
		echo ""
		return 0
	else
		echo ""
		fail "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
		fail "x                                                                               x"
		fail "x           audio playback and recording functions are abnormal                 x"
		fail "x                                                                               x"
		fail "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
		echo ""
		return -1
	fi
}

activate_exit(){
	echo "`date +'%Y-%m-%d %H:%M:%S'`--error. get lock fail. there is other audio_test.sh running. will be exit."
	exit 1
}

get_lock(){
	lock_file_name="/tmp/`basename $0`.pid"
	exec 6<>${lock_file_name}
	flock -n 6 || activate_exit
	#echo "`date +'%Y-%m-%d %H:%M:%S'`--ok. get lock success. start running audio_test.sh."
	echo "$$">&6
}

is_ro_fs()
{
	local tst_file="$PWD/`basename $0`.ro"
	touch $tst_file
	if [ $? -ne 0 ]; then
		echo "The current directory is read-only"
		exit
	fi
	rm $tst_file
}

trap 'echo "exit.."; killall arecord aplay $(basename $0)' SIGINT

usage()
{
	echo "**** List of PLAYBACK Hardware Devices ****"
	aplay -l | grep card
	echo ""
	echo "Usage:"
	echo " audio_test.sh [l|1]"
	echo "	 l : software loopback test"
	echo "		 audio_test.sh l				#HDMI paly default audio file /media/eee.wav"
	echo "		 audio_test.sh l <wav_file>			#HDMI play custom file"
	echo ""
	echo "	 1 : cycle stress test, all available sound cards"
	echo "		 audio_test.sh 1 [0/1/2/3]			#option sound card"
	echo "		 audio_test.sh 1				#all sound card"
	echo ""
	echo "	 e : external loopback test"
	echo "		 audio_test.sh e [0/1/2/3]			#default 0"

	echo ""
	echo "example:"
	echo "	arecord -D plughw:0,0 -d 5 -r 48000 -f S16_LE -c 2 -t wav /tmp/aaa.wav"
	echo "	aplay   -D plughw:0,0 /tmp/aaa.wav"
	exit 1
}

#main
get_lock $@

is_ro_fs

ret=0
echo "args: $1, $2"
case "$1" in
	"l")
		loopback_test $2
		ret=$?
		;;
	"1")
		loop_stree_test $2
		ret=$?
		;;
	"e")
		ext_loopback_test $2
		ret=$?
		;;
	"e1")
		ii=1
		while [ 1 ]; do
			notify "\n=========================-------------------------- ext_loopback_test ii=$ii"
			ext_loopback_test $2 || break
			ii=$((ii + 1))
		done
		;;
	*)
		usage
		;;
esac

exit $ret
