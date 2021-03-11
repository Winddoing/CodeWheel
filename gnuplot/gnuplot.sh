#!/bin/bash
##########################################################
# File Name		: gnuplot.sh
# Author		: winddoing
# Created Time	: 2021年03月11日 星期四 14时14分32秒
# Description	:
##########################################################

## 要提取的日志的关键字信息，可以搜索多个，空格分开
search_str="RENDER"

function gen_gnuplot_input {
    colors=(a7001f 0a60c2 b2582b 21a66c d6604d 4393c3 f4a582 92c5de edcbb7 b1c5d0)

    grep $search_str $1 | awk '{print NR","$3 $6 $12}' > /tmp/$search_str.csv

    echo -n "
    set key box;
    set datafile separator \",\";
    set terminal png size 1440,900 font \"Arial,9\";
    set xlabel \"count\";
    set ylabel 'usage (%)';
    set yrange [0:100];
    set samples 50;
    set output \"$1.png\";
    set style data points;
    set xtics 10;
    set ytics 10;
    set grid;"

    i=0
    echo -n  "plot"
    (( ++i )) && echo -n "'/tmp/$search_str.csv' using 1:2 with line lc rgb \"#${colors[$(($i % 10))]}\" title \"RENDER\","
    (( ++i )) && echo -n "'/tmp/$search_str.csv' using 1:3 with line lc rgb \"#${colors[$(($i % 10))]}\" title \"VIDEO1\","
    (( ++i )) && echo -n "'/tmp/$search_str.csv' using 1:4 with line lc rgb \"#${colors[$(($i % 10))]}\" title \"VIDEO2\","
}

## 将第一次终端传入参数，即要分析的日志文件名，传到函数中，处理后，用gnullot画图
gen_gnuplot_input $1  | gnuplot -persist
