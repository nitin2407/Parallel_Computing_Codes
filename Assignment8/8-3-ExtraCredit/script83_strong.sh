#!/bin/bash
g++ -std=c++11 -o seq matrix_mult_seq.cpp
mpic++ -std=c++11 -o par83 8-3.cpp

n[0]=16384
n[1]=74000
n[2]=148000
power=20
c=1
set -x
for ((i=0;i<3;i=i+1));
do
	seq=$(./seq ${n[$i]} $power)
	for ((threads=1;threads<=16;threads=threads*4));
	do
		par=$(mpirun -n $threads ./par83 ${n[$i]} $power)
		echo $threads $(echo "$seq/$par"|bc -l)>>../data/out83_strong_$c.dat
	done
	gnuplot -e "inp = '../data/out83_strong_$c.dat'; out='../plots/plot83_strong_$c.png'" ../plot.sh
	c=$((c+1))
done
set +x