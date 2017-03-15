/*
    PC Extra Credit Assignment 2  Sequential N-Queens Implementation
    Author: Sharan Girdhani(800960333)
*/

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstdlib>
#include <sys/time.h>
using namespace std;
    
int n;
double t1;

double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t0 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t0;
}

// function to check availability of a particular spot
bool isAvailable(int queens[], int col)
{
       
    for(int i=0;i<col;i++)
    { 
        if((queens[i] == queens[col]) || (abs(queens[col] - queens[i]) == (col - i))) 
        {   
            return false;
        }
    }
    return true;
}

// Function to go back one step(backtrack)
void go_back(int &col)
{
    col--;
    if(col==-1)
    {
        // Program done. Calculating the final time and making an exit
        double t2 = record_time();
        cout << (t2-t1) << endl;
        exit(1);
    }
    return;
}

    
int main(int argc, char *argv[])
{
    // checking for the command line arguments
    if(argc != 2)
    {
        cout << "Wrong input format" << endl;
        return 0;
    }

    n = atoi(argv[1]);
    int*  queen = new int[n];
    int c = 1;
    long long int count=0;
    queen[0]=0;

    bool source_from_backtrack=false;

    // start time
    t1 = record_time();

    // While loop to run until all the possibilities of nqueens are considered
    while(1)
    {
        // While loop that runs until we find a successful NQueens occurence
        while(c < n)
        {
            if(!source_from_backtrack)
            {
                queen[c] = -1;
            }
            source_from_backtrack=false;
            while(queen[c]<n)
            {
                queen[c]++;
                while(queen[c]==n)
                {
                    go_back(c);
                    queen[c]++;
                }

                if(isAvailable(queen, c))
                {
                    break;
                }                  
            }
            c++;
        }
        count++; // found a successful implementation of N-Queens
     
        go_back(c); // go back a step and perform the whole process again
        source_from_backtrack=true;
    }
    return 0;
}
