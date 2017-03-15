#!/bin/bash
g++ -std=c++11 -o seq heat_eq_seq.cpp
mpic++ -std=c++11 -o par81 8-1.cpp

n[0]=5120
n[1]=16384
n[2]=32768
power=20
c=1
set -x
for ((i=0;i<3;i=i+1));
do
	seq=$(./seq ${n[$i]} $power)
	for ((threads=1;threads<=32;threads=threads*2));
	do
		par=$(mpirun -n $threads ./par81 ${n[$i]} $power)
		echo $threads $(echo "$seq/$par"|bc -l)>>../data/out81_strong_$c.dat
	done

	gnuplot -e "inp = '../data/out81_strong_$c.dat'; out='../plots/plot81_strong_$c.png'" ../plot.sh
	c=$((c+1))
done
set +x
