/*
	PC Assignment 8 Q1-Heat Equation Algorithm - Horizontal Data Partition
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot81_strong_(1,2,3).png and plot81_weak_(1,2,3).png in the plots folder for Strong scaling and weak scaling experiments respectively
	Associated Data: out81_strong_(1,2,3).dat and out81_weak_(1,2,3).dat in the data folder for Strong scaling and weak scaling experiments respectively
	
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

#define MAX_NUMBER 1000
using namespace std;

int n,P, power;
double *H;
const int root=0;

// recording current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}

// Fill the H array with random values
void rand_fill_values()
{
	for(int i = 0;i<n; i++)
	{
		for(int j = 0;j<n;j++)
		{
			H[i*n + j] = (rand()%MAX_NUMBER + 1)*1.0;
		}
 	}
 	return;
}

void print()
{
	for(int i = 0;i<n;i++)
	{
		for(int j = 0;j<n;j++)
		{
			cout << H[i*n + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	return;
}

// standard function to calculate the value based on different conditions
double calculate_value(int i, int j)
{
	double result = 0.0;
	if(i == 0 || i == n-1 || j == 0 || j == n-1)
	{
		if((i == 0 && j == 0))
		{
			result = 6*H[i*n + j] + H[i*n + j+1] + H[(i+1)*n + j] + H[(i+1)*n + j+1];
		}
		else if(i == 0 && j == n-1)
		{
			result = 6*H[i*n + j] + H[i*n + j-1] + H[(i+1)*n + j] + H[(i+1)*n + j - 1];
		}
		else if(i == n-1 && j == 0)
		{
			result = 6*H[i*n + j] + H[i*n + j+1] + H[(i-1)*n +j] + H[(i-1)*n + j+1];
		}
		else if(i == n-1 && j == n-1)
		{
			result = 6*H[(i)*n + j] + H[(i-1)*n + j] + H[i*n + j-1] + H[(i-1)*n + j-1];
		}
		else if(i == 0)
		{
			result = H[i*n + j-1] + 4*H[i*n + j] + H[i*n + j+1] + H[(i+1)*n + j-1] + H[(i+1)*n + j] + H[(i+1)*n + j+1];
 		}
 		else if(i == n-1)
		{
			result = H[(i-1)*n + j-1] + H[(i-1)*n + j] + H[(i-1)*n + j+1] + H[i*n + j-1] + 4*H[i*n + j] + H[i*n + j+1];
 		}
 		else if(j == 0)
		{
			result = H[(i-1)*n + j] + H[(i-1)*n + j+1] + 4*H[i*n + j] + H[i*n + j+1] + H[(i+1)*n + j] + H[(i+1)*n + j+1];
 		}
 		else if(j == n-1)
		{
			result = H[(i-1)*n + j-1] + H[(i-1)*n + j] + H[i*n + j-1] + 4*H[i*n + j] + H[(i+1)*n + j-1] + H[(i+1)*n + j];
 		}
	}
	else
	{
		result = H[(i-1)*n + j-1]+H[(i-1)*n + j]+H[(i-1)*n + j+1]+H[i*n + j-1]+H[i*n + j]+H[i*n + j+1]+H[(i+1)*n + j-1]+H[(i+1)*n + j]+H[(i+1)*n + j+1];
	}
	return result/9.0;
}

// Function to calculate the Heat Equation
void Heat_Equation(int rank)
{
	int k;
	double *G = new double[n*(n/P)];
	int start = (rank)*n*n/P; // Start row of the processor
	int end = (rank+1)*n*n/P; // End row of the processor
	int x_power = 0;

	// if(rank == 0)
	// {
	// 	cout << "H power 0: " << endl;
	// 	print();
	// }

	while(power--)
	{
		// computing y = Ax
		k = 0;
		int j,l;
		for(int i = start; i<end;i++) 
		{
			j = i%n;
			l = i/n;
			G[k] = calculate_value(l,j);
			k++;
		}

		if(rank != 0)
		{
			MPI_Send(G, n*(n/P), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); // Sending the value to root node
		}
		else
		{
			for(int i = start;i<end;i++)
			{
				H[i] = G[i];
			}

			int starti, endi;
			for(int i = 1;i<P;i++)
			{
				starti = (i)*n*n/P;
				endi = (i+1)*n*n/P;
				// Recieving the value from other nodes in 0 and appending to its result
				MPI_Recv(G, n*(n/P), MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				k = 0;
				for(int j = starti;j<endi;j++)
				{
					H[j] = G[k];
					k++;
				}
			}
		}
		
		// finally broadcasting the updated value to all the nodes
		MPI_Bcast(H, n*n, MPI_DOUBLE, root, MPI_COMM_WORLD);
    	x_power++;

  //   	if(rank == 0)
		// {
		// 	cout << "H power " << x_power << ":"<< endl;
		// 	print();
		// }
	}
	//print(x);
	delete[] G;
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
	H = new double [n*n];

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
	Heat_Equation(rank);

	// Close MPI
	MPI_Finalize();

	double t2 = record_time();
	if(rank == 0)
		cout << (t2-t1) << endl;
	delete[] H;
	return 0;
}