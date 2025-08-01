set terminal png size 1920,1080  # 设置图像大小为 1920x1080
set output "bs.png"
set title "test"
set grid

set xlabel 'sepal length'
set ylabel 'petal length'

set datafile separator ','

# 绘制数据
plot 'bs.csv' using 1:3 title "pkt-size" with linespoints, \
     'bs.csv' using 1:(stringcolumn(4) eq "I" ? $3 : 1/0) with lines title "I" lw 8 lt 3 lc rgb "red"
