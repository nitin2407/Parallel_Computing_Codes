/*
   PC Assignment 5 Q1
   Author: Sharan Girdhani(800960333)
   Associated Plot: plot51_(1,2).png in the plots folder
   Associated Data: out51_(1,2).dat in the data folder
   The related Assignment 4 plot can be found as plot51_Ass4.png in the plots folder.

   The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <limits.h>
#include <omp.h>

#define MAX_NUMBER 1000
// #define NUM_ELEM 100

using namespace std;

int *a,n,*thread_min;

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
	#pragma omp parallel for
	for(int i = 0;i<n; i++)
	{
		a[i] = rand()% MAX_NUMBER + 1;
 	}
}

int min2(int a1, int a2)
{
	return a1 <= a2 ? a1 : a2;
}

void find_min_task(int *a,int index, int gran)
{
	int t_no = omp_get_thread_num();
	int local_min = thread_min[t_no];
	for (int i = index; i < index+gran; i++)
	{
		local_min = min2(local_min, a[i]);
	}

	if(local_min < thread_min[t_no])
		thread_min[t_no] = local_min;
	return;
}

void find_mininum(int *a,int n, int gran)
{
	for(int i=0;i<n;i+=gran)
	{
		#pragma omp task
		find_min_task(a, i, gran);
	}

	#pragma omp taskwait
	return;
}


void print()
{
	for(int i = 0; i< n; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	if(argc != 4)
	{
		cout << "No of inputs has to be 3 or 4" << endl;
		return 0;
	}

	n = stoi(argv[1]);
	a =new int[n];
	
	int num_proc = stoi(argv[2]);
	thread_min = new int[num_proc+1];

	int gran = stoi(argv[3]);
	for(int i = 0;i<=num_proc;i++)
	{
		thread_min[i] = INT_MAX;
	}

	rand_fill_values();

	// time starts
 	clock_t t0 = clock();
 	double t1=record_time();

	// print();
	omp_set_num_threads(num_proc);
	#pragma omp parallel
	{
		#pragma omp single
		{
			find_mininum(a, n, gran);
		}
	}
	#pragma omp taskwait
	int ultimate_min = INT_MAX;
	for(int i = 0;i<=num_proc;i++)
	{
		ultimate_min = min2(ultimate_min, thread_min[i]);
	}

	double t2=record_time();
	// cout << ultimate_min << endl;
	cout << (t2-t1) << endl;
	delete[] a;
	return 0;
}
