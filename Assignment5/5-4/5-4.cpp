/*
   PC Assignment 5 Q4
   Author: Sharan Girdhani(800960333)
   Associated Plot: plot54_1.png in the plots folder
   Associated Data: out54_1.dat in the data folder

   The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <limits.h>
#include <omp.h>

using namespace std;

#define STANDARD_SIZE 8
#define SIZE_T 4
#define RATIO 5

const long long int MAX_NUMBER = 100;


// structure of a task
struct task
{
   char type; // to determine the type like M,N,P,T,C etc.
   int value; // the value of the node
   int pred; // no of predecessors pending
   int is_done; // is_done taken for 3 stages: 0 -> pending, 1 -> in progress, 2 -> completed
   int index; // index of the task needed for special cases
   task *suc; // successor node of a task
};

struct task *m[STANDARD_SIZE], *n[STANDARD_SIZE], *p[STANDARD_SIZE], *t[SIZE_T], *C[3][3]; //all required tasks
int opint, total_tasks = 28;


// function for recording the current time
double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t1 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t1;
}


// multiplication of 2 random numbers
int mult()
{
   int temp = 0, c;
   int a = rand() % MAX_NUMBER + 1;
   int b = rand() % MAX_NUMBER + 1;
   while(temp++ < opint*RATIO)
      c = a*b;
   return c;
}

// addition of 2 random numbers
int sum()
{
   int temp = 0, c;
   int a = rand() % MAX_NUMBER + 1;
   int b = rand() % MAX_NUMBER + 1;

   while(temp++ < opint)
      c = a+b;
   return c;
}

// dynamically allocating memory to the tasks and initializing the values for m, n, p, t, C
void allocate_tasks()
{
   #pragma omp parallel for
   for(int i = 1;i< STANDARD_SIZE; i++)
   {
      m[i] = new task;
      m[i]->pred = 0;
      m[i]->is_done = 0;
      m[i]->type = 'M';
      m[i]->value = 0;
      m[i]->index = i;


      n[i] = new task;
      n[i]->pred = 0;
      n[i]->is_done = 0;
      n[i]->type = 'N';
      n[i]->value = 0;
      n[i]->index = i;

      p[i] = new task;
      p[i]->pred = 2;
      p[i]->is_done = 0;
      p[i]->type = 'P';
      p[i]->value = 0;
      p[i]->index = i;

      if(i < SIZE_T)
      {
         t[i] = new task;
         t[i]->pred = 2;
         t[i]->is_done = 0;
         t[i]->type = 'T';
         t[i]->value = 0;
         t[i]->index = i;
      }
      m[i]->suc = p[i];
      n[i]->suc = p[i];
   }

   #pragma omp parallel for
   for(int i = 1; i< 3; i++)
   {
      for(int j = 1; j < 3; j++)
      {
         C[i][j] = new task;
         C[i][j]->pred = 2;
         C[i][j]->is_done = 0;
         C[i][j]->suc = NULL;
         C[i][j]->type = 'C';
         C[i][j]->value = 0;
         C[i][j]->index = i;
      }
   }

   m[4]->pred++;
   n[4]->pred++;

   p[1]->suc = C[1][1];
   p[2]->suc = C[1][1];
   p[3]->suc = t[3];
   p[4]->suc = t[1];
   p[5]->suc = t[2];
   p[6]->suc = C[1][2];
   p[7]->suc = C[2][1];

   t[1]->suc = t[2];
   t[2]->suc = C[2][2];
   t[3]->suc = C[1][2];
   return;
}

// freeing memory after the program is run
void free_memory()
{
   for(int i = 1;i< STANDARD_SIZE; i++)
   {
      delete m[i];
      delete n[i];
      delete p[i];
      if(i < SIZE_T)
      {
         delete t[i];
      }
   }

   for(int i = 1; i< 3; i++)
   {
      for(int j = 1; j < 3; j++)
      {
         delete C[i][j];
      }
   }
   return;
}


// function checking whether all the tasks are done not
bool done()
{
   if(total_tasks == 0)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

// function to reduce a task from total_tasks after allocation
void reduce_task()
{
   total_tasks--;
   return;
}

// function to find the next available task
struct task *next_available_task()
{
   while(!done())
   {
      
      for(int i = 1; i< STANDARD_SIZE; i++)
      {
         if(m[i]->pred == 0 && m[i]->is_done == 0)
         {
            m[i]->is_done = 1;
            return m[i];
         }
      }

      for(int i = 1; i< STANDARD_SIZE; i++)
      {
         if(n[i]->pred == 0 && n[i]->is_done == 0)
         {
            n[i]->is_done = 1;
            return n[i];
         }
      }

      for(int i = 1; i< STANDARD_SIZE; i++)
      {
         if(p[i]->pred == 0 && p[i]->is_done == 0)
         {
            p[i]->is_done = 1;
            return p[i];
         }
      }
      for(int i = 1; i< SIZE_T; i++)
      {
         if(t[i]->pred == 0 && t[i]->is_done == 0)
         {
            t[i]->is_done = 1;
            return t[i];
         }
      }

      for(int i = 1; i< 3; i++)
      {
         for(int j = 1; j < 3; j++)
         {
            if(C[i][j]->pred == 0 && C[i][j]->is_done == 0)
            {
               C[i][j]->is_done = 1;
               return C[i][j];
            }
         }
      }
   }

   return NULL;
}

// function to do the task allocated to a particular thread
void do_task(struct task *data)
{
   switch(data->type)
   {
      case 'M':
      case 'N':
      case 'T':
      case 'C':
         data->value = sum();
         break;
      case 'P':
         data->value = mult();
         break;
   }

   // special cases with multiple successors
   if(data->type == 'M' && data->index == 3)
   {
      m[4]->pred--;
   }

   if(data->type == 'N' && data->index == 3)
   {
      n[4]->pred--;
   }

   if(data->type == 'T' && data->index == 1)
   {
      t[3]->pred--;
   }

   if(data->type == 'P' && data->index == 1)
   {
      t[1]->pred--;
   }
   if(data->type == 'P' && data->index == 3)
   {
      C[2][2]->pred--;
   }

   if(data->type == 'T' && data->index == 2)
   {
      C[2][1]->pred--;
   }

   if(data->suc != NULL)
   {
      data->suc->pred--;
      data->is_done = 2;
   }
   return;
}

// main function to pick up task and doing it.
void scheduler()
{
   while(1)
   {
      struct task *data = next_available_task();
      if(data == NULL)
         return;
      //cout << data->type << data->index << endl;
      reduce_task();
      
      #pragma omp task
      do_task(data);
      #pragma omp taskwait
   }
}

// MAIN function
int main(int argc, char *argv[]) 
{
   // Checking if the number of arguments were correct
   if(argc != 3)
   {
      cout << "Please enter operation intensity and number of threads as a parameter" << endl;
      return 0;
   }

   srand(time(NULL));

   allocate_tasks();

   // time starts
   clock_t t0 = clock();
   double t1 = record_time();

   opint = stoi(argv[1]); // Operation Intensity
   int NUM_THREADS = stoi(argv[2]); // Number of threads

   omp_set_num_threads(NUM_THREADS);
   #pragma omp parallel
   {
      #pragma omp single
      {
         scheduler();
      }
   }
   #pragma omp taskwait

   // recording final time
   double t2 = record_time();

   free_memory(); // Freeing all the memory allocation done during the entire program

   cout << (t2-t1) << endl; // Printing the time difference
   return 0;
}
