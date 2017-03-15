/*
	PC Assignment 7 Q4-Integration MPI Advanced Master Worker Algorithm
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot74_(1,2,3,4).png in the plots folder
	Associated Data: out74_(1,2,3,4).dat in the data folder

	All the Plots generated are Speedup(Y-Axis) Vs No. Processes(X-Axis)

	The plots generated can be a bit confusing. Please refer to the data file or the script file in case needed.
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

// Calculating the next for the counter which assigns the starting position of each work
int next(int counter)
{
	if(counter + chunk < n)
		counter += chunk;
	else
		counter = n;
	return counter;
}

// recording current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}

// Function for master
void Num_Integration_Master()
{
	MPI_Status status;
	MPI_Request req_s, req_r;
	int counter = 0;
	double sum_r, sum=0.0;
	int current_pending = 0;
	int last_send_value = -2; // value sent to all the worker processors to end
	
	// Sending 3 tasks to each worker using Isend
	for(int i=1;i<P;i++)
	{
		for(int j = 0;j<3;j++)
		{
			if(counter < n)
			{
				MPI_Isend(&counter, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &req_s);
				current_pending++;
				counter = next(counter);
			}
		}
		if(counter >= n)
			break;
	}

	// Continuing recieving and sending back to the same worker till all the works are done
	while(counter < n)
	{
		MPI_Irecv(&sum_r,1,MPI_DOUBLE, MPI_ANY_SOURCE,2,MPI_COMM_WORLD,&req_r);
    	MPI_Wait(&req_r, &status);
		current_pending--;
		sum += sum_r;
		MPI_Isend(&counter, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &req_s);
		current_pending++;
		counter = next(counter);
	}
	// Recieving for the remaining pending works by other workers
	while(current_pending--)
	{
		MPI_Irecv(&sum_r,1,MPI_DOUBLE, MPI_ANY_SOURCE,2,MPI_COMM_WORLD,&req_r);
    	MPI_Wait(&req_r, &status);
		sum += sum_r;
	}

	// Sending value -2 to close the processors
	for(int i=1;i<P;i++) 
	{
		MPI_Isend(&last_send_value, 1, MPI_INT, i,0, MPI_COMM_WORLD, &req_s);
	}
	MPI_Wait(&req_s, &status); //  waiting for the processor to complete sending work
	sum = sum*t;
	//cout << sum << endl;
	return;
}

// Function for workers
void Num_Integration_Worker()
{
	int *recieve_data = new int[n];
	memset(recieve_data, -1, n*sizeof(int));
	MPI_Status *status;
	double sum,c;
	int start, end;
	MPI_Request req_recv[n];
	MPI_Request req_s;
	int cur_index = 0, do_index = 0;

	for(int i = 0; i< 3;i++)
	{
		MPI_Irecv(&recieve_data[cur_index],1,MPI_DOUBLE, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&req_recv[cur_index]);
		cur_index++;
	}

	MPI_Wait(&req_recv[0], MPI_STATUS_IGNORE);
	while(recieve_data[do_index] != -2)
	{
		sum = 0.0;
		start = recieve_data[do_index];
		if(start + chunk < n)
			end = start+chunk;
		else
			end = n;
		for (int i = start; i< end; i++) 
		{
			c = ((i+0.5)*t) + a;
			sum += func(c);
		}
		MPI_Isend(&sum, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &req_s);
		do_index++;

		MPI_Irecv(&recieve_data[cur_index],1,MPI_DOUBLE, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&req_recv[cur_index]);
		cur_index++;

		if(recieve_data[do_index] == -1)
		{
			MPI_Wait(&req_recv[do_index], MPI_STATUS_IGNORE);
		}
	}
	delete[] recieve_data;
	return;
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

	if(rank == 0)
	{
		Num_Integration_Master();
	}
	else
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
