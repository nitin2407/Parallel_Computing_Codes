/*
   PC Assignment 3 Q2-Sequential Algorithm
   Author: Sharan Girdhani(800960333)
   Associated Plot: plot321_1.png in the plots folder
   Associated Data: out321_1.dat in the data folder

   The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

#define STANDARD_SIZE 8
#define SIZE_T 4

const long long int MAX_NUMBER = 100;

int m[STANDARD_SIZE], n[STANDARD_SIZE], p[STANDARD_SIZE], t[SIZE_T], C[3][3], opint, RATIO;

// function for recording the current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}

// multiplying 2 numbers
int mult(int a, int b)
{
   int temp = 0, c;
   while(temp++ < opint*RATIO)
      c = a*b;
   return c;
}

// Adding 2 numbers
int sum(int a, int b)
{
   int temp = 0, c;
   while(temp++ < opint)
      c = a+b;
   return c;
}

// generate random values to the array m
void generate_m()
{
   for(int i=1; i<STANDARD_SIZE; i++)
   {
      m[i] = rand() % MAX_NUMBER + 1;
      if(i==4)
      {
         m[i] = sum(m[i-1], m[i]); // for special case 4, according to the given picture
      }
   }
   return;
}

// generate random values to the array n
void generate_n()
{
   for(int i=1; i<STANDARD_SIZE; i++)
   {
      n[i] = rand() % MAX_NUMBER + 1;
      if(i==4)
      {
         n[i] = sum(n[i-1], n[i]); // for special case 4, according to the given picture
      }
   }
   return;
}

// generate multiplication values to the array p according to the given picture
void generate_p()
{
   for(int i=1; i<STANDARD_SIZE; i++)
   {
      p[i] = mult(m[i],n[i]);
   }
   return;
}

// generate addition values to the array t according to the given picture
void generate_t()
{
   t[1] = sum(p[1], p[4]);
   t[2] = sum(t[1], p[5]);
   t[3] = sum(t[1], p[3]);
   return;
}

// generate addition values to the 2-d array C according to the given picture
void generate_c()
{
   C[1][1] = sum(p[1], p[2]);
   C[1][2] = sum(t[3], p[6]);
   C[2][1] = sum(t[2], p[7]);
   C[2][2] = sum(t[2], p[3]);
   return;
}

// MAIN Function
int main(int argc, char *argv[]) 
{

   // Checking if the number of arguments were correct
   if(argc != 3)
   {
      cout << "Please enter operation intensity and ratio as a parameter" << endl;
      return 0;
   }

   opint = stoi(argv[1]);
   RATIO = stoi(argv[2]);

   srand(time(NULL));
   // time starts
   clock_t t0 = clock();
   double t1 = record_time();
   
   generate_m(); // task 1
   generate_n(); // task 2
   generate_p(); // task 3
   generate_t(); // task 4
   generate_c(); // task 5
   
   // Recording final time
   double t2 = record_time();

   
   printf("%f\n", t2-t1); // Printing the time difference
   return 0;
}