#!/bin/bash
g++ -std=c++11 -o sequential 3-21.cpp
ratio=5
c=1
set -x
for ((opint=1000;opint<=100000000;opint=opint*10));
do
	seq=$(./sequential $opint $ratio)
    echo $opint $seq>>./data/out321_$c.dat
done
echo >>./data/out321_$c.dat
gnuplot -e "inp = './data/out321_$c.dat'; out='./plots/plot321_$c.png'" ../plot.sh
set +x