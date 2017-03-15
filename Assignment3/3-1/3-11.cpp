/*
	PC Assignment 3 Q1-Sequential Algorithm
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot311_(1,2,3,4).png in the plots folder
	Associated Data: out311_(1,2,3,4).dat in the data folder

	The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/


#include <iostream>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

using namespace std;

int f;
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

// MAIN Function
int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		cout << "Program expects 4 input parameters along with ./a.out" << endl;
		return 0;
	}
	// time starts
    clock_t t0 = clock();
	double t1 = record_time();
	
	// function to run
	int a = stoi(argv[1]), b = stoi(argv[2]), n = stoi(argv[3]), i, j;
	f = stoi(argv[4]);
	double t = ((b-a)*1.0)/(n*1.0), sum = 0.0, tt;
	
	for(i=0;i<n;i++)
	{
		double c = ((i+0.5)*t) + a;
		sum += func(c);
	}
	sum = sum*t;

	// time ends
	double t2= record_time();
	tt = (t2 - t1);

	// cout << "Number of Points: " << n << endl;
	// cout << "Operational Intensity: " << f << endl;
	// cout << "Time Taken: " << tt << " Seconds "<< endl;
	// cout << "Result: " << sum << endl;
	// cout << endl;
	// cout << endl;

	//cout << tt << endl;
	printf("%f\n", tt);

	return 0;
}
