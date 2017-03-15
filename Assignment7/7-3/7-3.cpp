/*
	PC Assignment 7 Q3-Integration MPI Master Worker Algorithm
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot73_(1,2,3,4).png in the plots folder
	Associated Data: out73_(1,2,3,4).dat in the data folder

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

int a,b,n,f,P, chunk;
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

// Function for the master processor
void Num_Integration_Master()
{
	MPI_Status status;
	int counter = 0;
	double sum_r, sum=0.0;

	// Initially sending work(sum of a chunk) all the workers
	for(int i=1;i<P;i++)
	{
		MPI_Send(&counter, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		if(counter + chunk < n)
			counter += chunk;
		else
			counter = n;
	}

	// Recieving the sum from different workers and sending back work to the same worker until all the works are done
	while(counter < n)
	{
		MPI_Recv(&sum_r, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		sum += sum_r;

		MPI_Send(&counter, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
		counter += chunk;
	}

	// Recieving the last work from all the workers and then sending MPI Tag as 2 inorder to inform them to return as all the works are done
	for(int i=1;i<P;i++) 
	{
		MPI_Recv(&sum_r, 1, MPI_DOUBLE, MPI_ANY_SOURCE,0, MPI_COMM_WORLD, &status);
		sum += sum_r;
		MPI_Send(0, 0, MPI_INT, i,2, MPI_COMM_WORLD);
	}

	sum = sum*t;
	//cout << sum << endl;
	return;
}

// Function for the worker processors
void Num_Integration_Worker()
{
	double sum,c;
	int start, end;
	MPI_Status status;

	while(1)
	{
		// Revieving the work from master
		MPI_Recv(&start, 1, MPI_INT, 0, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

		// checking if the MPI Tag is 2 which is the signal to return
		if (status.MPI_TAG == 2) 
		{
			return;
		}
		
		// Doing the work
		sum = 0.0;
		if(start + chunk < n)
			end = start+chunk;
		else
			end = n;
		for (int i = start; i< end; i++) 
		{
			c = ((i+0.5)*t) + a;
			sum += func(c);
		}
		// Sending the calculated work back to the master
		MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		cout << "Program expects 3 input parameters" << endl;
		return 0;
	}

	a = 1;
	b = 100;
	n = atoi(argv[1]);
	f = atoi(argv[2]);
	chunk = atoi(argv[3]);

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

	if(rank == 0) // master
	{
		Num_Integration_Master();
	}
	else // Worker
	{
		Num_Integration_Worker();
	}

	// Close MPI
	MPI_Finalize();

	double t2 = record_time();
	if(rank == 0)
		cout << (t2-t1) << endl;
	return 0;
}
