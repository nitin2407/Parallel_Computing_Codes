/*
	PC Assignment 8 Q2-Matrix Multiplication - Horizontal Data Partition
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot82_strong_h_(1,2,3).png and plot82_weak_h_(1,2,3).png in the plots folder for Strong scaling and weak scaling experiments respectively
	Associated Data: out82_strong_h_(1,2,3).dat and out82_weak_h_(1,2,3).dat in the data folder for Strong scaling and weak scaling experiments respectively
	
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
		x[i] = rand()%MAX_NUMBER + 1;
		//x.push_back(rand()% MAX_NUMBER + 1);
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

void Dense_Horizontal(int rank)
{
	int k;
	int start = (rank)*n/P;
	int end = (rank+1)*n/P;

	long long int *temp = new long long int[n/P + 1]; // For recieving data from other nodes
	long long int *y = new long long int[n/P +1]; // For sending the calculated data to 0
	int x_power = 0;

	// Declaring memory only for the part of A for each processor(Saves the memory a lot)
	A = new int*[end-start];	
	for(int i = 0; i < (end-start); i++)
	{
		A[i] = new int[n];
		for(int j = 0;j<n;j++)
		{
			A[i][j] = rand()%MAX_NUMBER + 1;
		}
	}
	
	// if(rank == 0)
	// {
	// 	cout << "x power 0: " << endl;
	// 	print();
	// }

	while(power--)
	{
		// computing y = Ax 
		k = 0;
		for(int i = 0; i<(end-start);i++) 
		{
			y[k]=0;
			for(int j = 0;j<n;j++) 
			{
				y[k] += A[i][j]*x[j];
			}
			k++;
		}
		int len = k;

		if(rank != 0)
		{
			MPI_Send(y, len, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
		}
		else
		{
			for(int i = start;i<len;i++)
			{
				x[i] = y[i];
			}
			int starti, endi, count;

			for(int i = 1;i<P;i++)
			{
				starti = (i)*n/P;
				endi = (i+1)*n/P;

				MPI_Recv(temp, (endi-starti), MPI_LONG_LONG_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				k = 0;
				for(int j = starti;j<endi;j++)
				{
					x[j] = temp[k];
					k++;
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
	Dense_Horizontal(rank);

	// Close MPI
	MPI_Finalize();

	double t2 = record_time();
	if(rank == 0)
		cout << (t2-t1) << endl;
	delete[] *A;
	delete[] x;
	return 0;
}