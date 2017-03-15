/*
   PC Assignment 5 Q3
   Author: Sharan Girdhani(800960333)
   Associated Plot: plot53_1.png in the plots folder
   Associated Data: out53_1.dat in the data folder
  
    The related Assignment 4 plot can be found as plot53_Ass4.png in the plots folder.

   The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

#define MAX_NUMBER 1000

using namespace std;

// recording current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}

void rand_fill_values(int *a, int n)
{
    #pragma omp parallel for
    for(int i = 0;i<n; i++)
    {
      a[i] = rand()% MAX_NUMBER + 1;
    }
}

void merge(int *a, int n, int *temp) 
{
    int i = 0, j = n/2, k = 0;
    // Merge same as we do in sequential algorithm
    while (i<n/2 && j<n) 
    {
        if (a[i] < a[j]) 
        {
           temp[k++] = a[i++];
        } 
        else 
        {
           temp[k++] = a[j++];
        }
    }
    while (i<n/2) 
    {
        temp[k++] = a[i++];
    }
     
    while (j<n) 
    {
        temp[k++] = a[j++];
    }
    // replacing a with the newly merged temp array
    memcpy(a, temp, n*sizeof(int));
    return;
}

void mergesort(int *a, int n, int *temp)
{
  if (n < 2)
  return;

   #pragma omp task
   mergesort(a, n/2, temp);
   #pragma omp taskwait

   #pragma omp task
   mergesort(a+(n/2), n-(n/2), temp);
 
   #pragma omp taskwait

   merge(a, n, temp);
}

// For testing purposes
void print(int *a, int n)
{
  for(int i = 0; i< n; i++)
  {
    cout << a[i] << " ";
  }
  cout << endl;
}


int main(int argc, char const *argv[])
{
    srand(time(NULL));

    if(argc != 3)
    {
      cout << "No of inputs has to be number of integers in array and number of threads" << endl;
      return 0;
    }

    int n = stoi(argv[1]);
    int *a =new int[n];
    int *temp = new int[n];

    int num_proc = stoi(argv[2]);

    rand_fill_values(a,n);
    //print(a,n);
    // time starts
    clock_t t0 = clock();
    double t1=record_time();
    
    omp_set_num_threads(num_proc);
    #pragma omp parallel
    {
        #pragma omp single
        mergesort(a, n, temp);
    }

    double t2 = record_time();
    //print(a,n);
    cout << (t2-t1) << endl;
    delete[] a;
    return 0;
}
