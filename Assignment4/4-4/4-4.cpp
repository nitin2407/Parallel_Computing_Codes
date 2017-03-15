/*
	PC Assignment 4 Q4
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot44_1.png in the plots folder
	Associated Data: out44_1.dat in the data folder

	The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <omp.h>
#include <limits.h>

#define MAX_NUMBER 1000

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
	int num_proc = stoi(argv[2]), search_element = rand()% MAX_NUMBER + 1;
	int index = -1;
	int* a =new int[n];
	rand_fill_values(a);

	// time starts
 	clock_t t0 = clock();
 	double t1=record_time();

 	omp_set_num_threads(num_proc);
 	//int rec[num_proc] = {0};
	int rec[num_proc+1];
 	for(int i = 0; i<= num_proc;i++)
 	{
 		rec[i] = 0;
 	}

	#pragma omp parallel for schedule(dynamic, 100000)
	for(int i = 0;i<n; i++)
	{
		int thread_no = omp_get_thread_num();
		if(a[i] == search_element)
		{				
			if(rec[thread_no] == 0) //storing the first occurence of each thread
			{
				rec[thread_no] = i;
			}
		}
	 }

	int min = INT_MAX; // max limit of int possible
	for(int i = 1; i<=num_proc; i++) // selecting the min index among all threads
	{
		if(min > rec[i] && rec[i] != 0)
		{
			min = rec[i];
		}
	}

	// time ends
	double t2=record_time();
	cout << (t2-t1) << endl;
	delete[] a;
	return 0;
}
