/*
	PC Assignment 4 Q6
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot46_1.png in the plots folder
	Associated Data: out46_1.dat in the data folder

	The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

void rand_fill_values(int *a)
{
	for(int i = 0;i<n; i++)
	{
		a[i] = rand()% MAX_NUMBER + 1;
 	}
 	return;
}

int min2(int a1, int a2)
{
	return a1 < a2 ? a1 : a2;
}

// Merge function (same as normal merge sort)
void merge(int *a, int l, int m, int r)
{
	int n1 = m - l + 1, n2 = r - m;

	// Creating left and right arrays
	int L[n1], R[n2];
	for (int i = 0; i < n1; i++)
	{
		L[i] = a[l + i]; // from l to l+n1-1
	}
	for (int j = 0; j < n2; j++)
	{
		R[j] = a[m + 1+ j]; // from m+1 to m+n2
	}

	
	int i = 0, j = 0, k = l;

	// Merging 2 arrays of length n1 & n2
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			a[k++] = L[i++];
		}
		else
		{
			a[k++] = R[j++];
		}
	}

	if(j == n2) // in case R finishes first, leftover elements of L will get inserted as it is
	{
		while (i < n1)
		{
			a[k++] = L[i++];
		}
	}
	else if(i == n1) // in case L finishes first, leftover elements of R will get inserted as it is
	{
		while (j < n2)
		{
			a[k++] = R[j++];
		}
	}
	return;
}

// Merge sort in an iterative manner
void merge_sort(int *a)
{
	for (int len=1; len<=n-1; len = 2*len)
	{
		#pragma omp parallel for schedule(dynamic, 100000)
		for (int ls=0; ls<n-1; ls += 2*len)
		{
			int mid = min2(ls+len-1, n-1);
			int right = min2(ls+(2*len)-1, n-1);
			merge(a, ls, mid, right);
		}
	}
	return;
}

// Main function
int main(int argc, char *argv[])
{
	n = stoi(argv[1]);
	int num_proc = stoi(argv[2]);

	int* a =new int[n];
	
	rand_fill_values(a);

	omp_set_num_threads(num_proc);

	// time starts
 	clock_t t0 = clock();
 	double t1=record_time();

	merge_sort(a);
	
	// time ends
	double t2=record_time();

	cout << (t2-t1) << endl;
	delete[] a;
	return 0;
}