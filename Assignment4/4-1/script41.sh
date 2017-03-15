#!/bin/bash
g++ -std=c++11 -fopenmp -o par_41 4-1.cpp
n=100000000
c=1
set -x

seq=$(./par_41 $n 1 static)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_41 $n $thread static)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out41_$c.dat	
done
gnuplot -e "inp = '../data/out41_$c.dat'; out='../plots/plot41_$c.png'" ../plot.sh
c=$((c+1))



seq=$(./par_41 $n 1 dynamic 1)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_41 $n $thread dynamic 1)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out41_$c.dat	
done
gnuplot -e "inp = '../data/out41_$c.dat'; out='../plots/plot41_$c.png'" ../plot.sh
c=$((c+1))



seq=$(./par_41 $n 1 dynamic 1000)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_41 $n $thread dynamic 1000)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out41_$c.dat	
done
gnuplot -e "inp = '../data/out41_$c.dat'; out='../plots/plot41_$c.png'" ../plot.sh
c=$((c+1))



seq=$(./par_41 $n 1 dynamic 100000)
for ((thread=1;thread<=16;thread=thread*2));
do
	par=$(./par_41 $n $thread dynamic 100000)
	echo $thread $(echo "$seq/$par"|bc -l)>>../data/out41_$c.dat	
done
gnuplot -e "inp = '../data/out41_$c.dat'; out='../plots/plot41_$c.png'" ../plot.sh
c=$((c+1))

set +x