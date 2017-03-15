/*
   PC Assignment 5 Q2
   Author: Sharan Girdhani(800960333)
   Associated Plot: plot52_1.png in the plots folder
   Associated Data: out52_1.dat in the data folder
	
	The related Assignment 4 plot can be found as plot52_Ass4.png in the plots folder.
	
   The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <limits.h>
#include <omp.h>

#define MAX_NUMBER 1000

using namespace std;

int *a,n,*thread_first;
//int search_element = rand()% MAX_NUMBER + 1;
int search_element = 1001;
bool found = false;

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

void find_first_task(int *a,int index, int gran)
{
	int t_no = omp_get_thread_num();

	for (int i = index; i < index+gran; i++)
	{
		if(a[i] == search_element && thread_first[t_no] > i)
		{
			thread_first[t_no] = i;
			found = true;
		}
	}
	return;
}

void find_first(int *a,int n, int gran)
{
	for(int i=0;i<n;i+=gran)
	{
		#pragma omp task
		find_first_task(a, i, gran);
		if(found)
			i = n;
	}
	#pragma omp taskwait
	return;
}

// For testing purposes
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
		cout << "No of inputs has to be num of integers in array, number of processors and granularity" << endl;
		return 0;
	}

	n = stoi(argv[1]);
	a =new int[n];
	
	int num_proc = stoi(argv[2]);
	thread_first = new int[num_proc+1];

	int gran = stoi(argv[3]);
	
	for(int i = 0;i<=num_proc;i++)
	{
		thread_first[i] = INT_MAX;
	}

	rand_fill_values();
	
	// time starts
 	clock_t t0 = clock();
 	double t1=record_time();

	//print();
	omp_set_num_threads(num_proc);
	#pragma omp parallel
	{
		#pragma omp single
		{
			find_first(a, n, gran);
		}
	}
	#pragma omp taskwait
	// defined as INT_MAX as the first index will be less than n
	int ultimate_first = INT_MAX;
	for(int i = 0;i<=num_proc;i++)
	{
		ultimate_first = min2(ultimate_first, thread_first[i]);
	}

	double t2=record_time();

	cout << (t2-t1) << endl;
	delete[] a;
	return 0;
}
