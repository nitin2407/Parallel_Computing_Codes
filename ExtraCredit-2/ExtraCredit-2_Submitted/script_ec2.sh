#!/bin/bash
g++ -std=c++11 -o seq seq_nqueens.cpp
g++ -std=c++11 -o openmp -fopenmp openmp_nqueens.cpp

set -x
for ((n=4;n<=16;n=n+2));
do
	seq=$(./seq $n)
	seq=$( printf "%.10f" $seq )
	for ((threads=2;threads<=32;threads=threads*2));
	do
		par=$(./openmp $n $threads)
		par=$( printf "%.10f" $par )
		echo $threads $(echo "$seq/$par"|bc -l)>>data/outEC2_openmp_$n.dat
	done
	gnuplot -e "inp = 'data/outEC2_openmp_$n.dat'; out='plots/plotEC2_openmp_$n.png'; titl='SpeedUp Vs Processor Plot for n=$n'" plot.sh
done
set +x
