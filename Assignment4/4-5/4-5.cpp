/*
	PC Assignment 4 Q5
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot45_1.png in the plots folder
	Associated Data: out45_1.dat in the data folder

	The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <omp.h>

#define MAX_NUMBER 1000
#define policy_n 1000000

using namespace std;
int n;

// recording current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}	

void rand_fill_values(int *a)
{
	for(int i = 0;i<n; i++)
	{
		a[i] = rand()% MAX_NUMBER + 1;
 	}
 	return;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	if(argc != 3)
	{
		cout << "No of inputs has to be 3 or 4" << endl;
		return 0;
	}

	n = stoi(argv[1]);
	int num_proc = stoi(argv[2]);

	int* a =new int[n];

	rand_fill_values(a);
	int sum = 0;

	// time starts
 	clock_t t0 = clock();
 	double t1=record_time();

 	omp_set_num_threads(num_proc);

 	#pragma omp parallel
    {
        #pragma omp for schedule(dynamic, policy_n) // prefix sum for each threads of the array
        for (int i = 1;i < n; i++)
        {
                a[i] += a[i-1];
        }            
    }

    for (int i = policy_n; i < n; i=i+policy_n)
    {
        #pragma omp for schedule(static)
        for (int j = i;j < i+policy_n; j++) // adding the previous thread's prefix sums
        {
                a[j] += a[i-1];
        }
    }

	// time ends
	double t2=record_time();
	cout << (t2-t1) << endl;
	delete[] a;
	return 0;
}