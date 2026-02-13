# 设置输出
set terminal png size 1200,800 enhanced font "Arial,12"
set output 'statm_mult.png'

# 设置多图
set multiplot layout 4,2 title "Memory Usage Detailed View"

# 图1: VSZ
set title "VSZ"
set xlabel "Sample Number"
set ylabel "Memory (Page)"
plot "statm.txt" using 0:1 with lines lw 2 notitle

# 图2: RSS
set title "RSS"
set xlabel "Sample Number"
set ylabel "Memory (Page)"
plot "statm.txt" using 0:2 with lines lw 2 notitle

# 图3: Shared
set title "Shared"
set xlabel "Sample Number"
set ylabel "Memory (Page)"
plot "statm.txt" using 0:3 with lines lw 2 notitle

# 图4: Text
set title "Text"
set xlabel "Sample Number"
set ylabel "Memory (Page)"
plot "statm.txt" using 0:4 with lines lw 2 notitle

# 图5: Lib
set title "Lib"
set xlabel "Sample Number"
set ylabel "Memory (Page)"
plot "statm.txt" using 0:5 with lines lw 2 notitle

# 图6: Data
set title "Data"
set xlabel "Sample Number"
set ylabel "Memory (Page)"
plot "statm.txt" using 0:6 with lines lw 2 notitle

## 图7: Dirty
#set title "Dirty"
#set xlabel "Sample Number"
#set ylabel "Memory (Page)"
#plot "statm.txt" using 0:7 with lines lw 2 notitle

unset multiplot
