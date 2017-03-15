#!/bin/bash
g++ -std=c++11 -o seq ../Num_Int_Seq.cpp
mpic++ -std=c++11 -o par72 7-2.cpp
f=10
c=1
set -x

for ((n=1000;n<=1000000000;n=n*1000000));
do
	seq=$(./seq $n $f)
	for ((threads=1;threads<=32;threads=threads*2));
	do
		par=$(mpirun -n $threads ./par72 $n $f)
		echo $threads $(echo "$seq/$par"|bc -l)>>../data/out72_$c.dat
	done
	gnuplot -e "inp = '../data/out72_$c.dat'; out='../plots/plot72_$c.png'" ../plot.sh
	c=$((c+1))
done

set +x