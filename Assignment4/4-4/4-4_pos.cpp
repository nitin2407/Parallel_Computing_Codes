/*
	PC Assignment 4 Q4 - theta pos implementation
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot44_2.png in the plots folder
	Associated Data: out44_2.dat in the data folder

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
	int* a =new int[n];
	rand_fill_values(a);
	// time starts
 	clock_t t0 = clock();
 	double t1=record_time();
 	int index = -1, f = 0, k, j;
 	omp_set_num_threads(num_proc);
	for(int i = 0; i< n; i++)
	{
		k = 2*(i+1);
		#pragma omp parallel for schedule(dynamic, 1000)
		for(j = i; j < k+1; j++)
		{
			if(search_element == a[j])
			{
				f = 1;
				index = j;
			}
		}

		if(f == 1)
		{
			i = n;
		}
	}
	//cout << index << endl;
	// time ends
	double t2=record_time();
	cout << (t2-t1) << endl;
	delete[] a;
	return 0;
}
