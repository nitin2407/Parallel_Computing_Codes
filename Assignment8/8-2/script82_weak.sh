#!/bin/bash
mpic++ -std=c++11 -o par82_h 8-2_horizontal.cpp
mpic++ -std=c++11 -o par82_v 8-2_vertical.cpp
mpic++ -std=c++11 -o par82_b 8-2_block.cpp

n[0]=16384
n[1]=23172
n[2]=32768
power=20
c=1
set -x
for ((i=0;i<3;i=i+1));
do
	for ((threads=1;threads<=32;threads=threads*2));
	do
		temp=$(echo "sqrt ( $threads )*${n[$i]}" | bc -l) ;
		temp=$( printf "%.0f" $temp )
		par=$(mpirun -n $threads ./par82_h $temp $power)
		echo $threads $par>>../data/out82_weak_h_$c.dat
		par=$(mpirun -n $threads ./par82_v $temp $power)
		echo $threads $par>>../data/out82_weak_v_$c.dat
	done
	for ((threads=1;threads<=16;threads=threads*4));
	do
		temp=$(echo "sqrt ( $threads )*${n[$i]}" | bc -l) ;
		temp=$( printf "%.0f" $temp )
		temp=$(( $temp-$((temp%4)) ))
		par=$(mpirun -n $threads ./par82_b $temp $power)
		echo $threads $par>>../data/out82_weak_b_$c.dat
	done
	gnuplot -e "inp = '../data/out82_weak_h_$c.dat'; out='../plots/plot82_weak_h_$c.png'" ../plot.sh
	gnuplot -e "inp = '../data/out82_weak_v_$c.dat'; out='../plots/plot82_weak_v_$c.png'" ../plot.sh
	gnuplot -e "inp = '../data/out82_weak_b_$c.dat'; out='../plots/plot82_weak_b_$c.png'" ../plot.sh
	c=$((c+1))
done
set +x