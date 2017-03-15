#!/bin/bash
g++ -std=c++11 -fopenmp -o par_51 5-1.cpp
n=100000000
c=1
set -x

for ((gran=1000;gran<=100000;gran=gran*100));
do
	seq=$(./par_51 $n 1 $gran)
	for ((threads=1;threads<=16;threads=threads*2));
	do
		par=$(./par_51 $n $threads $gran)
		echo $threads $(echo "$seq/$par"|bc -l)>>../data/out51_$c.dat
	done
	gnuplot -e "inp = '../data/out51_$c.dat'; out='../plots/plot51_$c.png'" ../plot.sh
	c=$((c+1))
done

set +x
