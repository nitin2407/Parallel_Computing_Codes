#!/bin/bash
mpic++ -std=c++11 -o par81 8-1.cpp
n[0]=1160
n[1]=1638
n[2]=1800
power=20
c=1
set -x
for ((i=0;i<3;i=i+1));
do
	for ((threads=1;threads<=32;threads=threads*2));
	do
		temp=$(echo "sqrt ( $threads )*${n[$i]}" | bc -l) ;
		temp=$( printf "%.0f" $temp )
		temp=$(( $temp-$((temp%$threads)) ))
		par=$(mpirun -n $threads ./par81 $temp $power)
		echo $threads $par>>../data/out81_weak_$c.dat
	done
	gnuplot -e "inp = '../data/out81_weak_$c.dat'; out='../plots/plot81_weak_$c.png'" ../plot.sh
	c=$((c+1))
done
set +x
