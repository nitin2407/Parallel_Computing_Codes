#!/bin/bash
g++ -std=c++11 -fopenmp -o par_46 4-6.cpp
n=1000000000
c=1
set -x

seq=$(./par_46 $n 1)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_46 $n $thread)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out46_$c.dat	
done
gnuplot -e "inp = '../data/out46_$c.dat'; out='../plots/plot46_$c.png'" ../plot.sh
c=$((c+1))

set +x