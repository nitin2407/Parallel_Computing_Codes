/*
	PC Assignment 8 Q2-Matrix Multiplication - Vertical Data Partition
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot82_strong_v_(1,2,3).png and plot82_weak_v_(1,2,3).png in the plots folder for Strong scaling and weak scaling experiments respectively
	Associated Data: out82_strong_v_(1,2,3).dat and out82_weak_v_(1,2,3).dat in the data folder for Strong scaling and weak scaling experiments respectively
	
	All the strong scaling experiment Plots generated are Speedup(Y-Axis) Vs No. Processes(X-Axis) and the weak scaling experiment Plots generated are Time(in secs)(Y-Axis) Vs No. Processes(X-Axis)
	The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <mpi.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <vector>

#define MAX_NUMBER 10
using namespace std;

int n,P, power;
long long int *x;
int **A;
const int root=0;

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
		x[i] = rand()% MAX_NUMBER + 1;
		// for(int j = 0;j<n;j++)
		// {
		// 	A[i][j] = rand()%MAX_NUMBER + 1;
		// }
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

void Dense_Vertical(int rank)
{
	int k;
	long long int *temp = new long long int[n];
	int start = (rank)*n/P;
	int end = (rank+1)*n/P;
	long long int *y = new long long int[n];
	A = new int*[n];	
	for(int i = 0; i < n; i++)
	{
		A[i] = new int[end-start];
		for(int j = 0;j<(end-start);j++)
		{
			A[i][j] = rand()%MAX_NUMBER + 1;
		}
	}
	int x_power = 0;

	// if(rank == 0)
	// {
	// 	cout << "x power 0: " << endl;
	// 	print();
	// }

	while(power--)
	{
		// computing y = Ax 
		for(int i = 0; i<n;i++) 
		{
			y[i] = 0;
			k = 0;
			for(int j = start;j<end;j++)
			{
				y[i] += A[i][k]*x[j];
				k++;
			}
		}

		if(rank != 0)
		{
			MPI_Send(y, n, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
		}
		else
		{
			for(int i = 0;i<n;i++)
			{
				x[i] = y[i];
			}
			int starti, endi;

			for(int i = 1;i<P;i++)
			{
				starti = (i)*n/P;
				endi = (i+1)*n/P;

				MPI_Recv(temp, n, MPI_LONG_LONG_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				for(int j = 0;j<n;j++)
				{
					x[j] += temp[j];
				}
			}
		}

		MPI_Bcast(x, n, MPI_LONG_LONG_INT, root, MPI_COMM_WORLD);
    	x_power++;

		// if(rank == 0)
		// {
		// 	cout << "x power " << x_power << ":"<< endl;
		// 	print();
		// }
	}

	delete[] y;
	delete[] temp;
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

	x = new long long int[n];

	rand_fill_values();

	int rank;
	// time starts
    clock_t t0 = clock();
	double t1 = record_time();

	// Initiate MPI
	MPI_Init(&argc, &argv);

	// get the current processor
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// get total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	// Function to calculate the Matrix Multiplication
	Dense_Vertical(rank);

	// Close MPI
	MPI_Finalize();

	double t2 = record_time();
	if(rank == 0)
		cout << (t2-t1) << endl;
	delete[] *A;
	delete[] x;
	return 0;
}
