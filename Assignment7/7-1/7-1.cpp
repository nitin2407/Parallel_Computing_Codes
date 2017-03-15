/*
 * 	PC Assignment 7 Q1 Preliminary
 * 	Author: Sharan Girdhani(800960333)
 *
 * 	Run qsub -d `pwd` -l nodes=2:ppn=16 ./script71.sh to verify the result
*/

#include <mpi.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

#define MAX_SIZE 200


int main(int argc, char *argv[])
{
	int rank;
	MPI_Init(&argc, &argv);
	char *host = new char[MAX_SIZE];

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	gethostname(host, MAX_SIZE);
	cout << "rank: " << rank << ", Host: " << host << endl;
	MPI_Finalize();
	return 0;
}
