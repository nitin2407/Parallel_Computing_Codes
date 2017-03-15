/*
	PC Assignment 8 Q3-Matrix Multiplication  Block Partition - Extra Credit Question
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot83_strong_(1,2,3).png and plot83_weak_(1,2,3).png in the plots folder for Strong scaling and weak scaling experiments respectively
	Associated Data: out83_strong_(1,2,3).dat and out83_weak_(1,2,3).dat in the data folder for Strong scaling and weak scaling experiments respectively
	
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

int n,P, power,s;
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

void Dense_Block(int rank)
{
	MPI_Comm row_comm;
	MPI_Comm col_comm;
	int row_color = rank/s;
	int col_color = rank%s;
	int row_rank, row_size, col_rank, col_size;
	MPI_Comm_split(MPI_COMM_WORLD, row_color, rank, &row_comm);
	MPI_Comm_split(MPI_COMM_WORLD, col_color, rank, &col_comm);

	MPI_Comm_rank(row_comm, &row_rank);
	MPI_Comm_size(row_comm, &row_size);
	MPI_Comm_rank(col_comm, &col_rank);
	MPI_Comm_size(col_comm, &col_size);

	int k,l;
	long long int *temp = new long long int[n/s + 1]; //storing MPI_Reduce data for all the rows
	long long int *temp2 = new long long int[n/s +1];
	
	int starty = (rank%s)*n/s;
	int endy = (rank%s+1)*n/s;

	int startx = (rank/s)*n/s;
	int endx = (rank/s+1)*n/s;
	long long int *y = new long long int[n/s];

	// Declaring and assigning random numbers simultaneously only for the part of A required by the processor
	A = new int*[endx-startx+1];
	for(int i = 0; i < (endx-startx); i++)
	{
		A[i] = new int[endy-starty+1];
		for(int j = 0;j<(endy-starty);j++)
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
		k = 0;
		for(int i = 0; i<(endx-startx);i++) 
		{
			y[k] = 0;
			l = 0;
			for(int j = starty;j<endy;j++)
			{
				y[k] += A[i][l]*x[j];
				l++;
			}
			k++;
		}
		// Using the row communicator Summing all the value of the y array into the root node of the correspong row
		MPI_Reduce(y, temp, (endx-startx), MPI_LONG_LONG_INT, MPI_SUM, 0, row_comm);
		
		if(row_rank == 0)
		{
			if(rank == 0)
			{
				for(int i = startx;i<endx;i++)
				{
					x[i] = temp[i];
				}

				for(int i = 1;i<s;i++)
				{
					MPI_Recv(temp2, n/s, MPI_LONG_LONG_INT, i*s, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					int starti = i*n/s;
					int endi = (i+1)*n/s;
					k = 0;
					for(int j = starti;j<endi;j++)
					{
						x[j] = temp2[k];
						k++;
					}
				}
			}
			else
			{
				MPI_Send(temp, (endx-startx), MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
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
	// Freeing data
	MPI_Comm_free(&row_comm);
	MPI_Comm_free(&col_comm);
	//print(x);
	delete[] y;
	delete[] temp;
	delete[] temp2;
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
	s = sqrt(P);
	// Function to calculate the Matrix Multiplication
	Dense_Block(rank);

	// Close MPI
	MPI_Finalize();

	double t2 = record_time();
	if(rank == 0)
		cout << (t2-t1) << endl;

	delete[] *A;
	delete[] x;
	return 0;
}