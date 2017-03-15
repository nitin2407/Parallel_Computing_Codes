#!/bin/bash
mpic++ -std=c++11 -o par83 8-3.cpp

n[0]=16384
n[1]=23172
n[2]=32768
power=20
c=1
set -x
for ((i=0;i<3;i=i+1));
do
	for ((threads=1;threads<=16;threads=threads*4));
	do
		temp=$(echo "sqrt ( $threads )*${n[$i]}" | bc -l) ;
		temp=$( printf "%.0f" $temp )
		temp=$(( $temp-$((temp%4)) ))
		par=$(mpirun -n $threads ./par83 $temp $power)
		echo $threads $par>>../data/out83_weak_$c.dat
	done
	gnuplot -e "inp = '../data/out83_weak_$c.dat'; out='../plots/plot83_weak_$c.png'" ../plot.sh
	c=$((c+1))
done
set +x