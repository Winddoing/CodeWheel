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


function record_play_test()
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

function loop_play_or_record_test()
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

function loop_play_test()
{
	local card_id=$1
	local p_tst_dir="$TST_DIR/$card_id/p"

	echo "[$card_id] *** Test sound card: `cat /sys/class/sound/card$card_id/device/modalias`"

	echo "[$card_id] *** Prepare audio files PWD=$PWD ***"
	if [ ! -d $p_tst_dir ]; then
		mkdir -p $p_tst_dir
	fi
	rm $p_tst_dir/* -rf

	cp $SYS_TST_FILE $p_tst_dir -arpv
	cp $PWD/*.wav $p_tst_dir -arpv
	sync


	echo "[$card_id] ########## Cycle stress test Play card_id=$card_id ##########"
	echo "[$card_id] p_tst_dir:$p_tst_dir"
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
	done
	echo "[$card_id] ########### Cycle stress test over ##########"
}


function loop_stree_test()
{
	local tst_card_id=$1

	for id in $(seq 0 4)
	do
		modalias="/sys/class/sound/card${id}/device/modalias"
		[ -f $modalias ] || continue

		if [ $tst_card_id ]; then
			LOOP_TST_RP=1
			[ $tst_card_id == $id ] || continue;
		fi

		grep "rt5651" $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &
		grep "es8311" $modalias 2>&1 >/dev/null && loop_play_or_record_test $id &

		grep "hdmi-sound0" $modalias 2>&1 >/dev/null && loop_play_test $id &
		grep "hdmi-sound1" $modalias 2>&1 >/dev/null && loop_play_test $id &
	done

	return 0
}

function single_record_test()
{
	local sound_card_name=$1

	for id in $(seq 0 4)
	do
		modalias="/sys/class/sound/card${id}/device/modalias"
		[ -f $modalias ] || continue
		grep $sound_card_name $modalias 2>&1 >/dev/null && break
	done
	echo "------ Test sound card id=$id ------"
	record_play_test $id $PWD $PWD
}

#main
if [ x"$1" == x"p" ]; then
	tst_file=$2
	set -x
	aplay -D plughw:everestes8311co -r 48000 -f S16_LE -c 2 -t wav ${tst_file:-$SYS_TST_FILE}
	exit
elif [ x"$1" == x"rp" ]; then
	single_record_test "es8311"
	exit
elif [ x"$1" == x"1" ]; then
	loop_stree_test $2
	exit
elif [ x"$1" == x"k" ]; then
	set -x
	rm $TST_DIR -rf
	killall arecord aplay $(basename $0)
	exit
else
	echo "Usage:"
	echo " audio_test.sh [p|rp|1]"
	echo "	 p : play test, default es8311 codec"
	echo "		 audio_test.sh p				#paly default audio file /media/eee.wav"
	echo "		 audio_test.sh p <wav_file>			#play custom file"
	echo ""
	echo "	rp : record test, default es8311 codec"
	echo "		 audio_test.sh rp				#Record and Play test"
	echo ""
	echo "	 1 : cycle stress test, all available sound cards"
	echo "		 audio_test.sh 1 [0/1/2/3]			#option sound card"
	echo "		 audio_test.sh 1				#all sound card"
	echo ""
	echo "	 k : kill all process, only for audio_test.sh 1 loop test"

	echo ""
	echo "example:"
	echo "	arecord -D plughw:0,0 -d 5 -r 48000 -f S16_LE -c 2 -t wav /tmp/aaa.wav"
	echo "	aplay   -D plughw:0,0 /tmp/aaa.wav"
	exit
fi
