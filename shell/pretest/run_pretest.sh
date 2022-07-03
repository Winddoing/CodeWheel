#!/bin/bash

#---------------------#
PREFIX="-en \033["

CURSOR_HIDE="$PREFIX?25l"
CURSOR_SHOW="$PREFIX?25h"
WORD_RED=$PREFIX"31m"
WORD_GREEN=$PREFIX"32m"
WORD_YELLOW=$PREFIX"33m"
WORD_BLUE=$PREFIX"34m"
WORD_WHITE=$PREFIX"37m"
BLANK=$PREFIX"8m"
UNBLANK=$PREFIX"7m"
BUTTOM_LINE=$PREFIX"4m"
#---------------------#

MENU_WIDTH=50
PRETEST_DIR=pretest
func_return=

make_menuconfig(){
	local item
	local tmp

cat <<EOF > menu_config
	#menu_main
	name=`cat /proc/cpuinfo | grep machine | awk -F ":" '{print $2}' | awk '$1=$1'` Pretest
	Automatic test=all_test
EOF

	find $PRETEST_DIR -name "test_*" | {
		while read line; do
			item=${line:5}
			((tmp=${#item}-3))
			item=${item:0:$tmp}
			echo "Test "$item"="$line >> menu_config
			echo $line
		done
	}
}

set_pos() {
	echo $PREFIX"$2"";""$1"H
}

blank_space() {
	local space=
	for((i=1;i<=$1;i++));do
		space=$space' '
	done
	echo "$space"
	return 0
}

test_all() {
	local result=
	local item
	local tmp
	find $PRETEST_DIR -name "test_*" | {
		while read line; do
#			item=${line:5}
			item=${line:14}
			((tmp=${#item}-3))
			item=${item:0:$tmp}
			((tmp=20-tmp))
			tmp="$(blank_space tmp)"
#			echo $line
			if [ $line != 'test_exit.sh' ]
			then
				$line #&>/dev/null
				case "$?" in
				'0')
					result=""$result"  "$item""${tmp}"|\ttest OK!\n"
					;;
				'2')
					result=""$result"  "$item""${tmp}"|\ttest manual\n"
					;;
				*)
					result=""$result"  "$item""${tmp}"|\ttest Failed!\n"
					;;
				esac
			fi
		sleep 2
		done
		clear
		echo
		echo "-------------- Test result -------------"
		echo
		echo -e "$result"
	}
}


menu_echo() {
	for((i=1;i<=$2;i++));do
		echo -en "$1"
	done
	return 0
}

menu() {
	local para=$1
	shift
	local content=$*
	local left=
	local tmp=0
	((tmp=${#content}%2))
	[ "$tmp" = 1 ] && content="$content "

	[ "$para" = "title" ] && {
		((left=($MENU_WIDTH-${#content})/2))
		menu_echo " " 1
		menu_echo - left
		echo -en "$content"
		menu_echo - left
		echo -en "\n|"
		menu_echo " " MENU_WIDTH
		echo -en "|\n"
	}
	[ "$para" = "list" ] && {
		((left=$MENU_WIDTH-${#content}-9))
		echo -n "|"
		menu_echo " " 5
		echo -en "$current_list_num:"
		[[ current_list_num -gt 9 ]] && menu_echo " " 1 || menu_echo " " 2
		echo -n "$content"
		menu_echo " " left
		echo -en "|\n"
		((current_list_num++))
	}
	return 0
}

make_menu() {
	current_list_num=1
	local get_it=0

	while read line; do
		[ ${#line} != 0 ] && {
			if [ $get_it -eq 0 ]; then
				[ ${line:0:1} = '#' -a "$1" = "${line:1}" ] && get_it=1
			else
				[ "${line:0:1}" = "#" ] && break
				[ "${line%%=*}" = "name" ] && menu title "${line#*=}" || menu list "${line%%=*}"
			fi
		}
	done < "./menu_config"
	echo -en "|"
	menu_echo " " MENU_WIDTH
	echo -en "|\n"
	menu_echo " " 1
	menu_echo "-" MENU_WIDTH
	echo -en "\n"
	return 0
}

get_final() {
	current_list_num=1
	func_return=""
	local get_it=0
	while read line; do
		[ ${#line} != 0 ] && {
			if [ $get_it -eq 0 ]; then
				[ ${line:0:1} = '#' ] && [ "$1" = ${line:1} ] && get_it=1
			else
				[ "${line:0:1}" = "#" ] && break
				[ "${line%%=*}" != "name" ] && {
					[ $current_list_num -eq "$2" ] && func_return="${line#*=}" \
					&& break
					((current_list_num++))
				}
			fi
		}
	done < "./menu_config"
}

menu_handle() {
	local menu_next=
	local cmd=
	local item
	local tmp
	if [ "${func_return:0:4}" = "menu" ]; then
		menu_next=$func_return
		echo next menu: $menu_next
		menu_current=$menu_next
	else
		cmd=$func_return
#		echo func_return $func_return
		if [ "$cmd" = "all_test" ]; then
			echo "  Testing ......"
			test_all
#			continue
		else
			[ -f $cmd ] && {
				item=${cmd:5}
				((tmp=${#item}-3))
				item=${item:0:$tmp}
				$cmd #&>/dev/null
				case "$?" in
				'0')
					echo "  $item test OK! "
					;;
				'2')
					;;
				*)
					echo "  $item test failed! "
					;;
				esac
			}
		fi
		read -p "  press ENTER continue"
	fi
}

menu_get_value() {
	local list_num=
	clear
	make_menu "$1"
	echo -en "  please select one:  "
	read list_num
	expr $list_num + 0 &>/dev/null
	[ $? = 0 ] && get_final "$1" $list_num || return -1
}

#main

#sleep 2
menu_current=menu_main
#echo $WORD_RED

[ ! -f menu_config ] && make_menuconfig
while true; do
	menu_get_value $menu_current
	[ $? = 0 ] && menu_handle || exit
done

#main end

exit
