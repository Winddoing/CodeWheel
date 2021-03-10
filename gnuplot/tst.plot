set terminal png
set output "tst.png"
set title "test"
set grid

set xlabel 'sepal length'
set ylabel 'petal length'

plot 'tst.csv' using 1 "%lf" with linespoints
