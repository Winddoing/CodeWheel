#!/bin/bash
##########################################################
# File Name		: dapm_debug_host.sh
# Author		: winddoing
# Created Time	: 2022年02月18日 星期五 15时30分49秒
# Description	:
##########################################################

ASOC_TOP="/sys/kernel/debug/asoc"
HOST_TMP="tmp"
 
DAPM_ROUTE_PIC="${HOST_TMP}/dapm_route.png"
DAPM_ROUTE_DOT="${HOST_TMP}/dapm_route.dot"
DAPM_WIDGET_INFO="${HOST_TMP}/dapm_widget.info"
DAPM_WIDGET_LIST="${HOST_TMP}/dapm_widget.list"
 
mkdir -p ${HOST_TMP}
 
rm -rf ${DAPM_ROUTE_PIC}
rm -rf ${DAPM_ROUTE_DOT}
rm -rf ${DAPM_WIDGET_INFO}
rm -rf ${DAPM_WIDGET_LIST}
 
adb root
adb remount
 
IFS=$'\n'
 
function parse_widget_information()
{
	local state_tmp="${HOST_TMP}/state.tmp"
	adb shell "cat '$1'" > ${state_tmp}
 
	cat ${state_tmp} >> ${DAPM_WIDGET_INFO}
	echo >> ${DAPM_WIDGET_INFO}
 
	widget_name=$(awk 'BEGIN{FS=":"} {if(NR==1) {print $1} }'  ${state_tmp})
	widget_state=$(awk 'BEGIN{FS=":"} {if(NR==1) {print $2} }' ${state_tmp} | awk '{print $1}')
 
	#if [ x"Off" == x"${widget_state}" ] || [ x"Offforce" == x"{widget_state}" ]; then
	#	echo ""\"${widget_name}\""[style="filled", color="black", fillcolor="lightyellow"];" >> ${DAPM_ROUTE_DOT}
	#	widget_in=$(awk 'BEGIN{FS="\""; OFS="\""} /in/{if(NR >= 2) {if($4) { print("", $4, "->", "'"$widget_name"'", "") >> "'"$DAPM_ROUTE_DOT"'"} } }'  $state_tmp)
	#	widget_out=$(awk 'BEGIN{FS="\""; OFS="\""} /out/{if(NR >= 2) {if($4) {print ("", "'"$widget_name"'", "->", $4,"") >> "'"$DAPM_ROUTE_DOT"'"} } }' $state_tmp)
	#fi
 
	if [ x"On" == x"${widget_state}" ] || [ x"Onforce" == x"${widget_state}" ]; then
		echo ""\"${widget_name}\""[style="filled", color="black", fillcolor="palegreen"];" >> ${DAPM_ROUTE_DOT}
		widget_in=$(awk 'BEGIN{FS="\""; OFS="\""} /in/{if(NR >= 2) {if($4) { print("", $4, "->", "'"$widget_name"'", "") >> "'"$DAPM_ROUTE_DOT"'"} } }'  $state_tmp)
		widget_out=$(awk 'BEGIN{FS="\""; OFS="\""} /out/{if(NR >= 2) {if($4) {print ("", "'"$widget_name"'", "->", $4,"") >> "'"$DAPM_ROUTE_DOT"'"} } }' $state_tmp)
	fi
 
	rm -rf ${state_tmp}
}
 
function add_widget_to_list()
{
	local dapm_dir="$1"
	local widget_list="${HOST_TMP}/widget.tmp"
 
	adb shell "ls '${dapm_dir}'" > ${widget_list}
 
	for widget in $(cat ${widget_list})
	do
		if [ x"bias_level" != x"${widget}" ]; then
			echo "${dapm_dir}/${widget}" >> ${DAPM_WIDGET_LIST}
			parse_widget_information "${dapm_dir}/${widget}"
		fi
	done
 
	rm -rf ${widget_list}
}
 
function search_dapm_directory()
{
	local top="$1"
	local top_dir="${HOST_TMP}/top.tmp"
	local sub_dir="${HOST_TMP}/sub.tmp"
 
	adb shell "ls '${top}'" > ${top_dir}
 
	for sub in $(cat ${top_dir})
	do
		adb shell "stat '${top}/${sub}'" > ${sub_dir}
		attribute=$(cat "${sub_dir}")
		if [[ ${attribute} =~ "directory" ]]; then
			if [ x"dapm" == x"${sub}" ]; then
				echo "Found dapm dir (${top}/${sub})"
				add_widget_to_list "${top}/${sub}"
			else
				search_dapm_directory "${top}/${sub}"
			fi
		fi
	done
 
	rm -rf ${top_dir}
	rm -rf ${sub_dir}
}
 
function draw_dapm_route_picture()
{
	local dot_tmp="${HOST_TMP}/dot.tmp"
 
	if [ ! -f ${DAPM_ROUTE_DOT} ]; then
		return
	fi
 
	sort -k2n $DAPM_ROUTE_DOT | awk '{if ($0!=line) print;line=$0}'  > $dot_tmp
	awk 'BEGIN{print("digraph a{")} {print $0}' $dot_tmp > ${DAPM_ROUTE_DOT}
	echo "}" >> ${DAPM_ROUTE_DOT}
 
	echo
	echo -e '\e[0;33;1mPicture at' "$DAPM_ROUTE_PIC" '\e[0m'
	echo
 
	dot -Tpng ${DAPM_ROUTE_DOT} -o $DAPM_ROUTE_PIC
 
	rm -rf ${dot_tmp}
}
 
search_dapm_directory ${ASOC_TOP}
 
draw_dapm_route_picture
