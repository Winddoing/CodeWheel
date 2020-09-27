#!/bin/bash

function out_red() {
    echo -e "\e[01;31m$@\e[0m"
}
out_red "test read"

function out_yellow() {
    echo -e "\e[33m$@\e[0m"
}
out_yellow "test yellow"

function out_green() {
    echo -e "\e[32m$@\e[0m"
}
out_green "test green"

function out_blue() {
    echo -e "\e[34m$@\e[0m"
}
out_blue "test blue"



echo -e "\033[31m 红色字 \033[0m"

echo -e "\033[34m 黄色字 \033[0m"

echo -e "\033[41;33m 红底黄字 \033[0m"

echo -e "\033[41;37m 红底白字 \033[0m"

echo -e "\033[30m 黑色字 \033[0m"

echo -e "\033[31m 红色字 \033[0m"

echo -e "\033[32m 绿色字 \033[0m"

echo -e "\033[33m 黄色字 \033[0m"

echo -e "\033[34m 蓝色字 \033[0m"

echo -e "\033[35m 紫色字 \033[0m"

echo -e "\033[36m 天蓝字 \033[0m"

echo -e "\033[37m 白色字 \033[0m"
