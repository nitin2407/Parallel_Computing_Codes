/*
	PC Assignment 4 Q2
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot42_(1,2,3,4).png in the plots folder for the 4 types of schedule policies given
	Associated Data: out42_(1,2,3,4).dat in the data folder

	The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <omp.h>

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

int min2(int a1, int a2)
{
	return a1 <= a2 ? a1 : a2;
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
	if(argc < 4 || argc > 5)
	{
		cout << "No of inputs has to be 3 or 4" << endl;
		return 0;
	}

	n = stoi(argv[1]);
	int num_proc = stoi(argv[2]);
	char *sched_policy = argv[3];

	int policy_n = -1;

	if(argc == 5)
	{
		policy_n = stoi(argv[4]);
	}

	int* a =new int[n];
	int min = MAX_NUMBER;
	rand_fill_values(a);
	omp_set_num_threads(num_proc);

	// time starts
 	clock_t t0 = clock();
 	double t1=record_time();

 	if(argc == 5) // check in case of a number (gran) given with the schedule policy
 	{
 		if(sched_policy == "static")
 		{
 			#pragma omp parallel for schedule(static, policy_n)
	 		for(int i = 0;i<n; i++)
			{
				min = min2(a[i], min);
		 	}
 		}
 		else
 		{
 			#pragma omp parallel for schedule(dynamic, policy_n)
	 		for(int i = 0;i<n; i++)
			{
				min = min2(a[i], min);
		 	}
 		}
 	}
 	else
 	{
 		if(sched_policy == "static")
 		{
 			#pragma omp parallel for schedule(static)
	 		for(int i = 0;i<n; i++)
			{
				min = min2(a[i], min);
		 	}
 		}
 		else
 		{
 			#pragma omp parallel for schedule(dynamic)
	 		for(int i = 0;i<n; i++)
			{
				min = min2(a[i], min);
		 	}
 		}
 	}

	// time ends
	double t2=record_time();
	cout << (t2-t1) << endl;
	delete[] a;
	return 0;
}