/*
   PC Assignment 3 Q2-Parallel Algorithm
   Author: Sharan Girdhani(800960333)
   Associated Plot: plot322_(1,2).png in the plots folder
   Associated Data: out321_(1,2).dat in the data folder

   The plots generated can be a bit confusing. Please refer to the data file in case needed.
*/

#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

#define STANDARD_SIZE 8
#define SIZE_T 4
#define NUM_THREADS 16

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

pthread_mutex_t mut1;
pthread_mutex_t mut2;
pthread_mutex_t mut3;
pthread_mutex_t mut4;
pthread_mutex_t mut5;

struct task *m[STANDARD_SIZE], *n[STANDARD_SIZE], *p[STANDARD_SIZE], *t[SIZE_T], *C[3][3]; //all required tasks
int opint, RATIO, total_tasks = 28;


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
   pthread_mutex_lock (&mut3);
   if(total_tasks == 0)
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

// function to reduce a task from total_tasks after allocation
void reduce_task()
{
   pthread_mutex_lock (&mut3);
   total_tasks--;
   pthread_mutex_unlock (&mut3);
   return;
}

// function to find the next available task
struct task *next_available_task()
{
   while(!done())
   {
      
      for(int i = 1; i< STANDARD_SIZE; i++)
      {
         pthread_mutex_lock (&mut1);
         if(m[i]->pred == 0 && m[i]->is_done == 0)
         {
            m[i]->is_done = 1;
            pthread_mutex_unlock (&mut1);
            return m[i];
         }
         pthread_mutex_unlock (&mut1);
      }

      for(int i = 1; i< STANDARD_SIZE; i++)
      {
         pthread_mutex_lock (&mut1);
         if(n[i]->pred == 0 && n[i]->is_done == 0)
         {
            n[i]->is_done = 1;
            pthread_mutex_unlock (&mut1);
            return n[i];
         }
         pthread_mutex_unlock (&mut1);
      }

      for(int i = 1; i< STANDARD_SIZE; i++)
      {
         pthread_mutex_lock (&mut1);
         if(p[i]->pred == 0 && p[i]->is_done == 0)
         {
            p[i]->is_done = 1;
            pthread_mutex_unlock (&mut1);
            return p[i];
         }
         pthread_mutex_unlock (&mut1);
      }
      for(int i = 1; i< SIZE_T; i++)
      {
         pthread_mutex_lock (&mut1);
         if(t[i]->pred == 0 && t[i]->is_done == 0)
         {
            t[i]->is_done = 1;
            pthread_mutex_unlock (&mut1);
            return t[i];
         }
         pthread_mutex_unlock (&mut1);
      }

      for(int i = 1; i< 3; i++)
      {
         for(int j = 1; j < 3; j++)
         {
            pthread_mutex_lock (&mut1);
            if(C[i][j]->pred == 0 && C[i][j]->is_done == 0)
            {
               C[i][j]->is_done = 1;
               pthread_mutex_unlock (&mut1);
               return C[i][j];
            }
            pthread_mutex_unlock (&mut1);
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
         pthread_mutex_lock (&mut2);
         data->value = sum();
         pthread_mutex_unlock (&mut2);
         break;
      case 'P':
         pthread_mutex_lock (&mut2);
         data->value = mult();
         pthread_mutex_unlock (&mut2);
         break;
   }

   // special cases with multiple successors
   pthread_mutex_lock(&mut4);
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
   pthread_mutex_unlock(&mut4);
}

// thread's starting function to pick up task and doing it.
void *scheduler(void *param)
{
   while(1)
   {
      struct task *data = next_available_task();
      if(data == NULL)
         return NULL;
      reduce_task();
      do_task(data);
   }
}

// MAIN function
int main(int argc, char *argv[]) 
{
   // Checking if the number of arguments were correct
   if(argc != 3)
   {
      cout << "Please enter operation intensity and ratio as a parameter" << endl;
      return 0;
   }

   srand(time(NULL));

   allocate_tasks();

   // initializing mutex
   pthread_mutex_init(&mut1, NULL);
   pthread_mutex_init(&mut2, NULL);
   pthread_mutex_init(&mut3, NULL);
   pthread_mutex_init(&mut4, NULL);
   pthread_mutex_init(&mut5, NULL);

   // time starts
   clock_t t0 = clock();
   double t1 = record_time();

   opint = stoi(argv[1]); // Operation Intensity
   RATIO = stoi(argv[2]); // Ratio

   pthread_t processors[NUM_THREADS];

   int i;
   // Threads creation
   for(i=0;i<NUM_THREADS;i++)
   {
         pthread_create(&processors[i], NULL, scheduler, &total_tasks);
   }

   for (i=0; i < NUM_THREADS; i++)
   {
      pthread_join(processors[i], NULL );
   }

   // recording final time
   double t2 = record_time();

   pthread_mutex_destroy(&mut1);
   pthread_mutex_destroy(&mut2);
   pthread_mutex_destroy(&mut3);
   pthread_mutex_destroy(&mut4);
   pthread_mutex_destroy(&mut5);

   free_memory();

   cout << (t2-t1) << endl; // Printing the time difference
   return 0;
}
