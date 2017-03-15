#!/bin/bash
g++ -std=c++11 -o seq ../Num_Int_Seq.cpp
mpic++ -std=c++11 -o par73 7-3.cpp
f=10
c=1
set -x
for ((gran=10;gran<=1000;gran=gran*100));
do
	for ((n=1000;n<=1000000000;n=n*1000000));
	do
		seq=$(./seq $n $f)
		for ((threads=2;threads<=32;threads=threads*2));
		do
			par=$(mpirun -n $threads ./par73 $n $f $gran)
			echo $threads $(echo "$seq/$par"|bc -l)>>../data/out73_$c.dat
		done
		gnuplot -e "inp = '../data/out73_$c.dat'; out='../plots/plot73_$c.png'" ../plot.sh
		c=$((c+1))
	done
done

set +x