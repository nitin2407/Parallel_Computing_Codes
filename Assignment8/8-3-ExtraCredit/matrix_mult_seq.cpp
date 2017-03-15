/*
    PC Assignment 8 Q2 - Matrix Multiplication Sequential Algorithm 
    Author: Sharan Girdhani(800960333)
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

#define MAX_NUMBER 100

int n,power;
long long int *x;
int **A;
// recording current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}

void rand_fill_values()
{
    for(int i = 0;i<n; i++)
    {
        x[i] = rand()%MAX_NUMBER + 1;
        //x.push_back(rand()% MAX_NUMBER + 1);
        for(int j = 0;j<n;j++)
        {
            A[i][j] = rand()%MAX_NUMBER + 1;
        }
    }
    return;
}

void print()
{
    for(int i = 0;i<n;i++)
    {
        cout << x[i] << " ";
    }
    cout << endl;
    return;
}

int main(int argc, char *argv[])
{   
    int x_power = 0;

    srand(time(NULL));
    if(argc != 3)
    {
        cout << "Program expects 2 input parameter" << endl;
        return 0;
    }

    n = atoi(argv[1]);
    power = atoi(argv[2]);

    long long int *y = new long long int[n];

    x = new long long int[n];

    A = new int*[n];    
    for(int i = 0; i < n; i++)
    {
        A[i] = new int[n];
    }

    rand_fill_values();

    // time starts
    clock_t t0 = clock();
    double t1 = record_time();

    // cout << "x power 0: " << endl;
    // print();

    while(power--)
    {
        for(int i = 0;i<n;i++)
        {
            y[i] = 0;
            for(int j = 0;j<n;j++)
            {
                y[i] += A[i][j]*x[j]; 
            }
        }

        for(int i = 0;i<n;i++)
        {
            x[i] = y[i];
        }
        x_power++;

        // cout << "x power " << x_power << ":"<< endl;
        // print();
    }
    double t2 = record_time();
    cout << (t2-t1) << endl;
    delete[] *A;
    delete[] x;
    delete[] y;
    return 0;
}