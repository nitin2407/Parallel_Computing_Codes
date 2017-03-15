#!/bin/bash
g++ -std=c++11 -fopenmp -o par_43 4-3.cpp
n=100000000
c=1
set -x

for ((gran=10;gran<=1000;gran=gran*100));
do
	for ((n=1000;n<=1000000000;n=n*1000000));
	do
    	for ((sched=1;sched<=1000;sched=sched*1000));
    	do
    		seq=$(./par_43 1 $sched $n $gran)
    		for ((threads=1;threads<=16;threads=threads*2));
    		do
    			par=$(./par_43 $threads $sched $n $gran)
    			echo $threads $(echo "$seq/$par"|bc -l)>>../data/out43_$c.dat
    		done
    		gnuplot -e "inp = '../data/out43_$c.dat'; out='../plots/plot43_$c.png'" ../plot.sh
    		c=$((c+1))
    	done
    done
done

set +x
