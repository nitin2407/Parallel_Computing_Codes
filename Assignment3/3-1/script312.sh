#!/bin/bash
g++ -std=c++11 -o sequential 3-11.cpp
g++ -std=c++11 -lpthread -o parallel 3-12.cpp 

a=1
b=100
c=1
set -x

for ((n=10;n<=100000000;n=n*10));
do
	for ((opint=1;opint<=1000;opint=opint*10));
	do
        seq=$(./sequential $a $b $n $opint)
    	for ((gran=10;gran<=100000;gran=gran*100));
    	do
    		for ((threads=2;threads<=16;threads=threads*2));
    		do
    			par=`./parallel $a $b $n $opint 2 $gran $threads`
    			echo $threads $(echo "$seq/$par"|bc -l)>>./data/out312_$c.dat
    		done
    		echo >>./data/out312_$c.dat
    	done
    	gnuplot -e "inp = './data/out312_$c.dat'; out='./plots/plot312_$c.png'" ../plot.sh
    	c=$((c+1))
    done
done

set +x