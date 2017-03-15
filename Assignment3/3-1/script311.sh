#!/bin/bash
g++ -std=c++11 -o sequential 3-11.cpp
a=1
b=100
c=1
set -x

for ((opint=1;opint<=1000;opint=opint*10));
do
	for ((n=10;n<=100000000;n=n*10));
	do
		seq=$(./sequential $a $b $n $opint)
	    echo $seq $n>>./data/out311_$c.dat
	done
	echo >>./data/out311_$c.dat
	gnuplot -e "inp = './data/out311_$c.dat'; out='./plots/plot311_$c.png'" ../plot.sh
	c=$((c+1))
done
set +x