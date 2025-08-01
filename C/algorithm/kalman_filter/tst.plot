set terminal png
set output "tst.png"
set title "test"
set grid

set xlabel 'time'
set ylabel 'data'

set datafile separator ','

plot 'tst.csv' using 2 title "Raw" with linespoints, \
	'tst.csv' using 3 title "Filtered" with linespoints lc rgb "red"
