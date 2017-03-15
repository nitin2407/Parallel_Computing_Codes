#!/bin/bash
g++ -std=c++11 -fopenmp -o par_54 5-4.cpp
c=1
set -x

for ((op_int=10;op_int<=100000000;op_int=op_int*10));
do
	seq=$(./par_54 $op_int 1)
	par=$(./par_54 $op_int 16)
	echo $op_int $(echo "$seq/$par"|bc -l)>>../data/out54_$c.dat
done
gnuplot -e "inp = '../data/out54_$c.dat'; out='../plots/plot54_$c.png'" ../plot.sh
c=$((c+1))

set +x
