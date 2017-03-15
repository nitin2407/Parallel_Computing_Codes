#!/bin/bash
g++ -std=c++11 -fopenmp -o par_441 4-4.cpp
g++ -std=c++11 -fopenmp -o par_442 4-4_pos.cpp
n=1000000000
c=1
set -x

seq=$(./par_441 $n 1)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_441 $n $thread)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out44_$c.dat	
done
gnuplot -e "inp = '../data/out44_$c.dat'; out='../plots/plot44_$c.png'" ../plot.sh
c=$((c+1))


seq=$(./par_442 $n 1)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_442 $n $thread)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out44_$c.dat	
done
gnuplot -e "inp = '../data/out44_$c.dat'; out='../plots/plot44_$c.png'" ../plot.sh
c=$((c+1))

set +x