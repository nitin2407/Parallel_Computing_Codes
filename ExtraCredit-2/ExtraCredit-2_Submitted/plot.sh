reset
set term png
set output out
set title titl
set ylabel "Speedup"
set xlabel "Processor"
plot inp with linespoints ls 1 lt rgb "red"
