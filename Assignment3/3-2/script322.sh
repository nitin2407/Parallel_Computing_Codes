#!/bin/bash
g++ -std=c++11 -o sequential 3-21.cpp
g++ -std=c++11 -lpthread -o parallel 3-22.cpp
set -x
ratio=5
c=1
for ((opint=1000;opint<=100000000;opint=opint*10));
do
	seq=$(./sequential $opint $ratio)
	par=$(./parallel $opint $ratio)
	echo $opint $(echo "$seq/$par"|bc -l)>>./data/out322_$c.dat
    # echo $par $opint>>./data/out322_$c.dat
done
echo >>./data/out322_$c.dat
gnuplot -e "inp = './data/out322_$c.dat'; out='./plots/plot322_$c.png'" ../plot.sh


opint=100
k=2
for ((ratio=5;ratio<=50;ratio=ratio+5));
do
	seq=$(./sequential $opint $ratio)
	par=$(./parallel $opint $ratio)
	echo $ratio $(echo "$seq/$par"|bc -l)>>./data/out322_$k.dat
done
echo >>./data/out322_$k.dat
gnuplot -e "inp = './data/out322_$k.dat'; out='./plots/plot322_$k.png'" ../plot.sh
set +x