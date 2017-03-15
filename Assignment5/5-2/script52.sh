#!/bin/bash
g++ -std=c++11 -fopenmp -o par_52 5-2.cpp
n=100000000
c=1
set -x

seq=$(./par_52 $n 1 10000)
for ((threads=1;threads<=16;threads=threads*2));
do
	par=$(./par_52 $n $threads 10000)
	echo $threads $(echo "$seq/$par"|bc -l)>>../data/out52_$c.dat
done
gnuplot -e "inp = '../data/out52_$c.dat'; out='../plots/plot52_$c.png'" ../plot.sh
c=$((c+1))

set +x
