/*
    PC Assignment 8 Q1-Heat Equation Sequential Algorithm 
    Author: Sharan Girdhani(800960333)
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>

#define MAX_NUMBER 100
using namespace std;

int n, power;
double **H;

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
        for(int j = 0;j<n;j++)
        {
            H[i][j] = (rand()%MAX_NUMBER + 1)*1.0;
        }
    }
    return;
}

void print()
{
    for(int i = 0;i<n;i++)
    {
        for(int j = 0;j<n;j++)
        {
            cout << H[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    return;
}

double calculate_value(int i, int j)
{
    double result = 0.0;
    if(i == 0 || i == n-1 || j == 0 || j == n-1)
    {
        if((i == 0 && j == 0))
        {
            result = 6*H[i][j] + H[i][j+1] + H[i+1][j] + H[i+1][j+1];
        }
        else if(i == 0 && j == n-1)
        {
            result = 6*H[i][j] + H[i][j-1] + H[i+1][j] + H[i+1][j-1];
        }
        else if(i == n-1 && j == 0)
        {
            result = 6*H[i][j] + H[i][j+1] + H[i-1][j] + H[i-1][j+1];
        }
        else if(i == n-1 && j == n-1)
        {
            result = 6*H[i][j] + H[i-1][j] + H[i][j-1] + H[i-1][j-1];
        }
        else if(i == 0)
        {
            result = H[i][j-1] + 4*H[i][j] + H[i][j+1] + H[i+1][j-1] + H[i+1][j] + H[i+1][j+1];
        }
        else if(i == n-1)
        {
            result = H[i-1][j-1] + H[i-1][j] + H[i-1][j+1] + H[i][j-1] + 4*H[i][j] + H[i][j+1];
        }
        else if(j == 0)
        {
            result = H[i-1][j] + H[i-1][j+1] + 4*H[i][j] + H[i][j+1] + H[i+1][j] + H[i+1][j+1];
        }
        else if(j == n-1)
        {
            result = H[i-1][j-1] + H[i-1][j] + H[i][j-1] + 4*H[i][j] + H[i+1][j-1] + H[i+1][j];
        }
    }
    else
    {
        result = H[i-1][j-1]+H[i-1][j]+H[i-1][j+1]+H[i][j-1]+H[i][j]+H[i][j+1]+H[i+1][j-1]+H[i+1][j]+H[i+1][j+1];
    }
    return result/9.0;
}

void Heat_Equation()
{
    double **G = new double*[n];
    
    for(int i = 0; i < n; i++)
    {
        G[i] = new double[n];
    }

    int h_power = 0;

    //cout << "H power 0: " << endl;
    //print();

    while(power--)
    {
        // computing y = Ax 
        for(int i = 0; i<n;i++) 
        {
            for(int j = 0;j<n;j++)
            {
                G[i][j] = calculate_value(i,j);
            }
        }

        for(int i = 0; i<n;i++) 
        {
            for(int j = 0;j<n;j++)
            {
                H[i][j] = G[i][j];
            }
        }
        h_power++;

       // cout << "H power " << h_power << ":"<< endl;
       // print();
    }
    delete[] *G;
    return; 
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if(argc != 3)
    {
        cout << "Program expects 2 input parameter" << endl;
        return 0;
    }

    n = atoi(argv[1]);
    power = atoi(argv[2]);
    H = new double*[n];
    
    for(int i = 0; i < n; i++)
    {
        H[i] = new double[n];
    }

    rand_fill_values();
    
    // time starts
    clock_t t0 = clock();
    double t1 = record_time();
    // Function to calculate the Heat Equation
    Heat_Equation();

    double t2 = record_time();
    cout << (t2-t1) << endl;
    delete[] *H;
    return 0;
}
