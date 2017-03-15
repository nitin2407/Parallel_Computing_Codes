/*
	PC Assignment 3 Q1-Parallel Algorithm
	Author: Sharan Girdhani(800960333)
	Associated Plots: plot312_(1...32).png in the plots folder
	Associated Data: out312_(1...32).dat in the data folder

	The plots generated can be a bit confusing. Please refer to the data file in case needed.


	NOTE: for the special asked in the question 1-2(c part) i.e. speedup for granularity of 1000 and operational intensity of 10 is covered in the graphs plotted for the final q. But anyways Speedup is 2.751628.
*/

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>


using namespace std;

pthread_mutex_t mut1;
pthread_mutex_t mut2;
pthread_mutex_t mut3;

int a, b, n,f, type, counter = 0, CHUNK_SIZE, NUM_THREADS;
double t;

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

// function checking whether the loop is finished or not
bool done()
{
	pthread_mutex_lock (&mut3);
	if(counter == -1)
	{
		pthread_mutex_unlock (&mut3);
		return 1;
	}
	else
	{
		pthread_mutex_unlock (&mut3);
		return 0;
	}
}

// get next set of begin and end parameters for the thread to run
void getnext(int *beg, int *end)
{
	pthread_mutex_lock (&mut2);
	*beg = counter;
	if(counter + CHUNK_SIZE < n)
	{
		*end = counter + CHUNK_SIZE-1;
		counter += CHUNK_SIZE;
	}
	else
	{
		*end = n - 1;
		counter = -1;
	}
	pthread_mutex_unlock (&mut2);
	return;
}


// Method which adds only once to the shared variable per thread
void *allocate_aggregate_add(void *p)
{	
	double temp = 0.0;
	// done function
	while(!done())
	{
		int beg, end;
		// get next set of beg, end
		getnext(&beg, &end);
		
		for(int i = beg; i<= end; i++)
		{
			temp += func(((i+0.5)*t) + a);
		}
	}
	// Adding aggregately
	double *sum = (double *) p;
	pthread_mutex_lock (&mut1);
	*sum += temp;
	pthread_mutex_unlock (&mut1);
	return NULL;
}

// Method which adds to the shared variable at each step in a thread
void *allocate_each_step(void *p)
{	
	// done function
	while(!done())
	{
		int beg, end;
		// get next set of beg, end
		getnext(&beg, &end);

		double *sum = (double *) p;
		for(int i = beg; i<= end; i++)
		{
			// Mutex at each call
			pthread_mutex_lock (&mut1);
			*sum += func(((i+0.5)*t) + a);
			pthread_mutex_unlock (&mut1);
		}
	}
	return NULL;
}

// Method which adds to the shared variable once every call for getnext in a thread
void *allocate_once_per_call(void *p)
{	
	// done function
	while(!done())
	{
		int beg, end;
		// get next set of beg, end
		getnext(&beg, &end);
		
		double temp = 0.0;
		for(int i = beg; i<= end; i++)
		{
			temp += func(((i+0.5)*t) + a);
		}

		double *sum = (double *) p;
		pthread_mutex_lock (&mut1);
		*sum += temp;
		pthread_mutex_unlock (&mut1);
	}
	return NULL;
}

// MAIN Function
int main(int argc, char *argv[])
{

	// Checking if the number of arguments were correct
	if(argc != 8)
	{
		cout << "Program expects 7 input parameters along with ./a.out" << endl;
		return 0;
	}

	// time starts
    clock_t t0 = clock();
    double t1=record_time();

    // initializing mutex
	pthread_mutex_init(&mut1, NULL);
	pthread_mutex_init(&mut2, NULL);
	pthread_mutex_init(&mut3, NULL);

    // initializing values
	a = stoi(argv[1]); // A
	b = stoi(argv[2]); // B
	n = stoi(argv[3]); // C
	f = stoi(argv[4]); // Operational Intensity
	type = stoi(argv[5]); // type of method to call -- 1-> Sum each step, 2-> Sum once per call to getnext, 3-> only once per thread
	CHUNK_SIZE = stoi(argv[6]); // Granularity
	NUM_THREADS = stoi(argv[7]); // Number of threadss

	t = ((b-a)*1.0)/(n*1.0);

	pthread_t processors[NUM_THREADS];
	// function to run
	int i;
	double sum = 0.0, tt;

	// creating thread and calling the method based on the given type. For the plotting purpose type = 2 is used
	for(i=0;i<NUM_THREADS;i++)
	{
		switch(type)
		{
			case 1:
				pthread_create(&processors[i], NULL, allocate_each_step, &sum);
				break;
			case 2:
				pthread_create(&processors[i], NULL, allocate_once_per_call, &sum);
				break;
			case 3:
				pthread_create(&processors[i], NULL, allocate_aggregate_add, &sum);
				break;
			default:
				cout << "Invalid Parameter: Type" << endl;
				return 0;
		}
		
	}
	for (i=0; i < NUM_THREADS; i++)
	{
		pthread_join(processors[i], NULL );
	}

	sum = sum*t;

	// time ends
	double t2=record_time();
	tt = (t2 - t1);

	pthread_mutex_destroy(&mut1);
	pthread_mutex_destroy(&mut2);
	pthread_mutex_destroy(&mut3);

	// cout << "Number of Points: " << n << endl;
	// cout << "Operational Intensity: " << f << endl;
	// cout << "Time Taken: " << tt << " Seconds "<< endl;
	// cout << "Answer: " << sum << endl;
	// cout << endl;
	// cout << endl;
	printf("%f\n",tt); 
	return 0;
}