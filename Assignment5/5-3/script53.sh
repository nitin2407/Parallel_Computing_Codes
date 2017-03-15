#!/bin/bash
g++ -std=c++11 -fopenmp -o par_53 5-3.cpp
n=100000000
c=1
set -x

seq=$(./par_53 $n 1)
for ((threads=1;threads<=16;threads=threads*2));
do
	par=$(./par_53 $n $threads)
	echo $threads $(echo "$seq/$par"|bc -l)>>../data/out53_$c.dat
done
gnuplot -e "inp = '../data/out53_$c.dat'; out='../plots/plot53_$c.png'" ../plot.sh
c=$((c+1))

set +x
