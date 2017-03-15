#!/bin/bash
g++ -std=c++11 -fopenmp -o par_45 4-5.cpp
n=1000000000
c=1
set -x

seq=$(./par_45 $n 1)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_45 $n $thread)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out45_$c.dat	
done
gnuplot -e "inp = '../data/out45_$c.dat'; out='../plots/plot45_$c.png'" ../plot.sh
c=$((c+1))

set +x