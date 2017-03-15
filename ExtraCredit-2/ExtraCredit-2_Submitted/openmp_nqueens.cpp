/*
    PC Extra Credit Assignment 2  Open MP N-Queens Implementation
    Author: Sharan Girdhani(800960333)
    Associated Plots: plotEC2_openmp_{{n}}.png in the plots folder for Strong scaling experiment
    Associated Data: outEC2_openmp_{{n}}.dat in the data folder for Strong scaling experiment
    
    All the strong scaling experiment Plots generated are Speedup(Y-Axis) Vs No. Processes(X-Axis)
*/

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <omp.h>

using namespace std;

int n;
long long int count = 0;

double record_time()
{
    timeval tim;
    gettimeofday(&tim, NULL);
    double t0 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t0;
}

void markQueen(int queen[], int row, int col) 
{

    for(int i=0; i<row; i++) 
    {
        // check vertically
        if (queen[i]==col) 
        {
            return;
        }
        // check diagonally
        if (abs(queen[i]-col) == (row-i) ) 
        {
            return;
        }
    }

    // column is available, marking the position of queen here
    queen[row]=col;

    if(row==n-1) 
    {
        {
            // increasing the solution counter
            #pragma omp critical
                count++;
        }
    }
    else 
    {
        // try to fill next row
        for(int i=0; i<n; i++) 
        {
            markQueen(queen, row+1, i);
        }
    }
}

void nQueen() 
{
    #pragma omp parallel
    #pragma omp single
    {
        for(int i=0; i<n; i++) 
        {
            #pragma omp task
            markQueen(new int[n], 0, i);
        }
    }
}

int main(int argc, char*argv[]) 
{
    if(argc != 3) 
    {
        cout << "Wrong input format" << endl;
        return 0;
    }
    n = atoi(argv[1]);
    int num_proc = atoi(argv[2]);
    omp_set_num_threads(num_proc);
    
    double t1=record_time();
    nQueen();
    double t2=record_time();

    //cout << count << endl;
    cout << (t2-t1) << endl;
    return 0;
}
