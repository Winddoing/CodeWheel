# 设置输出
#set terminal png
#set terminal png size 1200,800 enhanced font "Arial,12"
#set output "statm.png"

set terminal svg enhanced size 1200,800 font "Arial,12"
set output "statm.svg"

# 设置标题和轴标签
set title "Memory Usage Over Time"
set xlabel "Sample Number"
set ylabel "Memory (Page)"

set key autotitle columnhead
set datafile separator whitespace

# 由于各列数值差异很大，使用对数坐标
set logscale y

# 设置网格
set grid

# 设置图例
set key outside right top

#plot 'rss.txt' using 1 "%lf" with linespoints
#plot "statm.txt" using 0:1 with linespoints lw 2 title "VSZ"
#plot "statm.txt" using 0:2 with linespoints lw 2 title "RSS"
#plot "statm.txt" using 0:2 with lines lw 2 title "RSS"

#plot "statm.txt" \
#        using 0:1 with lines lw 2 title "VSZ", \
#     "" using 0:2 with lines lw 2 title "RSS", \
#     "" using 0:3 with lines lw 2 title "Shared", \
#     "" using 0:4 with lines lw 2 title "Text", \
#     "" using 0:5 with lines lw 2 title "Lib", \
#     "" using 0:6 with lines lw 2 title "Data", \
#     "" using 0:7 with lines lw 2 title "Dirty"

plot "statm.txt" \
        using 0:1 with lines lw 2 title "VSZ", \
     "" using 0:2 with lines lw 2 title "RSS"
