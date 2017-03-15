#!/bin/bash
g++ -std=c++11 -o seq matrix_mult_seq.cpp
mpic++ -std=c++11 -o par82_h 8-2_horizontal.cpp
mpic++ -std=c++11 -o par82_v 8-2_vertical.cpp
mpic++ -std=c++11 -o par82_b 8-2_block.cpp

n[0]=16384
n[1]=74000
n[2]=148000
power=20
c=1
set -x
for ((i=0;i<3;i=i+1));
do
	seq=$(./seq ${n[$i]} $power)
	for ((threads=1;threads<=32;threads=threads*2));
	do
		par=$(mpirun -n $threads ./par82_h ${n[$i]} $power)
		echo $threads $(echo "$seq/$par"|bc -l)>>../data/out82_strong_h_$c.dat
		par=$(mpirun -n $threads ./par82_v ${n[$i]} $power)
		echo $threads $(echo "$seq/$par"|bc -l)>>../data/out82_strong_v_$c.dat
	done
	for ((threads=1;threads<=16;threads=threads*4));
	do
		par=$(mpirun -n $threads ./par82_b ${n[$i]} $power)
		echo $threads $(echo "$seq/$par"|bc -l)>>../data/out82_strong_b_$c.dat
	done

	gnuplot -e "inp = '../data/out82_strong_h_$c.dat'; out='../plots/plot82_strong_h_$c.png'" ../plot.sh
	gnuplot -e "inp = '../data/out82_strong_v_$c.dat'; out='../plots/plot82_strong_v_$c.png'" ../plot.sh
	gnuplot -e "inp = '../data/out82_strong_b_$c.dat'; out='../plots/plot82_strong_b_$c.png'" ../plot.sh
	c=$((c+1))
done
set +x