/*
	PC Assignment 7 Q2-Integration MPI Static Algorithm
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot72_(1,2).png in the plots folder
	Associated Data: out72_(1,2).dat in the data folder
	
	All the Plots generated are Speedup(Y-Axis) Vs No. Processes(X-Axis)

	The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <mpi.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

using namespace std;

int a,b,n,f,P;
double sum, t;
// function for Numerical Integration along using given operation intensity
double func(double x)
{
	for(int i = 0; i< f; i++)
	{
		x = x*x;
		x = sqrt(x);
	}
	return x*x;
}

// recording current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}

void Num_Integration_Static(int rank)
{
	sum = 0.0;
	double sum_r,c;

	// Selecting Begin and End of each processor based on their rank
	int begin = rank*(n/P);
	int end = (rank+1)*(n/P);

	// Calculation of sum in each processor
	for (int i = begin; i< end; i++) 
	{
		c = ((i+0.5)*t) + a;
		sum += func(c);
	}

	// Sending the sum to 0
	if (rank != 0) 
	{
		MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else 
	{
		for (int i=1; i<P; i++) 
		{ 
			// Recieving the sum from each processor and adding into its own result
			MPI_Recv(&sum_r, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += sum_r;
		}
		sum = sum*t;
		//cout << sum << endl;
	}
	return;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		cout << "Program expects 2 input parameters" << endl;
		return 0;
	}

	a = 1;
	b = 100;
	n = atoi(argv[1]);
	f = atoi(argv[2]);

	t = ((b-a)*1.0)/(n*1.0);

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

	// Function to calculate Numerical Integration
	Num_Integration_Static(rank);

	// Close MPI
	MPI_Finalize();

	double t2 = record_time();
	if(rank == 0)
		cout << (t2-t1) << endl;
	return 0;
}
