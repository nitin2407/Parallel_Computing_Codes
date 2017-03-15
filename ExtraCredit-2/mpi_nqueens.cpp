/*
    PC Extra Credit Assignment 2  MPI N-Queens Implementation
    Author: Sharan Girdhani(800960333)
    Associated Plots: plotEC2_mpi_{{n}}.png in the plots folder for Strong scaling experiment
    Associated Data: outEC2_mpi_{{n}}.dat in the data folder for Strong scaling experiment
    
    All the strong scaling experiment Plots generated are Speedup(Y-Axis) Vs No. Processes(X-Axis)
*/

#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <ctype.h>
#include <cstring>
#include <iostream>
#include <sys/time.h>

using namespace std;

long long int total_count  = 0;

short *Diag = NULL, *AntiD = NULL;

void Mark(int R, int C, int size, short *Diag, short *AntiD, bool Flag)
{
      Diag[R-C+size-1]=Flag;
      AntiD[R+C]=Flag;
      return;
}

void swap(int &x, int &y) 
{
   int temp = x;
   x = y;
   y = temp;
   return;
}

double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t0 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds
    return t0;
}

// Master process function:  sending tasks to workers to compute and receive results back.
void master(int n)
{  
   int col, k, comm_buffer[2], curr_count, limit = (n+1) / 2, nProc, curr_proc, nActive;
   MPI_Status Status;
   
   MPI_Comm_size (MPI_COMM_WORLD, &nProc);
   comm_buffer[0] = n;
   // Send initial configurations to all client processes --- or to those needed in case not all are required.
   for(col = 0, curr_proc = 1; curr_proc < nProc && col < limit; curr_proc++,col++)
   {  
         comm_buffer[1] = col;
         MPI_Send (comm_buffer, 2, MPI_INT, curr_proc, 1, MPI_COMM_WORLD);
   }
   
   nActive = curr_proc-1;    // Except master process
   
   if(curr_proc < nProc)    // More processes than jobs
   {
         int dmy[2] = {0, 0};   // Termination message to unused processes
         while (curr_proc < nProc)
         {
               MPI_Send (dmy, 2, MPI_INT, curr_proc++, 1, MPI_COMM_WORLD);
         }
   }

// Receive back results and send out new problems
   while ( col < limit )
   {
         MPI_Recv(&curr_count, 1, MPI_INT, MPI_ANY_SOURCE , 2, MPI_COMM_WORLD, &Status);
         curr_proc = Status.MPI_SOURCE;

         total_count  += curr_count;
         
         comm_buffer[1] = col++;
         
         MPI_Send(comm_buffer, 2, MPI_INT, curr_proc, 1, MPI_COMM_WORLD);
   }

   comm_buffer[0] = 0;
   while (nActive > 0)
   {
         MPI_Recv(&curr_count, 1, MPI_INT, MPI_ANY_SOURCE , 2, MPI_COMM_WORLD, &Status);
         nActive--;
         curr_proc = Status.MPI_SOURCE;

         total_count  += curr_count;
         MPI_Send(comm_buffer, 2, MPI_INT, curr_proc, 1, MPI_COMM_WORLD);
   }

   for (curr_proc = 1; curr_proc < nProc; curr_proc++)
   {
         MPI_Send(&curr_proc, 0, MPI_INT, curr_proc, 4, MPI_COMM_WORLD);
   }

   return;
}

int intncmp (int L[], int R[], int N)
{
   for (int i = 0; i < N; i++)
   {
      if ( L[i] - R[i] )
      {
         return L[i]-R[i];
      }
   }
   return 0;
}

void Rotate(int R[], int C[], int N, int Neg)
{
   int i,j;
   j = Neg ? 0 : N-1;
   for (i = 0; i < N; Neg ? j++ : j--)
   {
      C[i++] = R[j];
   }
   j = Neg ? N-1 : 0;
   for (i = 0; i < N; Neg ? j-- : j++)
   {
      R[C[i++]] = j;
   }
   return;
}

void Vmirror(int R[], int N)
{
   for (int i = 0; i < N; i++)
   {
      R[i] = (N-1) - R[i];
   }
   return;
}

// Function to check all the symmetries on the board
int SymmetryOps(int Board[], int Trial[], int Size)
{
   int i, Nequiv, *Scratch=&Trial[Size];

   for (i = 0; i < Size; i++)
   {
      Trial[i] = Board[i];
   }

   Rotate (Trial, Scratch, Size, 0);
   i = intncmp (Board, Trial, Size);
   if (i > 0)
   {
      return 0;
   }
   

   if ( i == 0 )
   {
      Nequiv = 1;
   }
   else
   {
      Rotate (Trial, Scratch, Size, 0);
      i = intncmp (Board, Trial, Size);
      if (i > 0)
      {
         return 0;
      }

      if ( i == 0 )
      {
         Nequiv = 2;
      }
      else
      {
         Rotate (Trial, Scratch, Size, 0);
         i = intncmp (Board, Trial, Size);
         if (i > 0)
         {
            return 0;
         }
         Nequiv = 4;
      }
   }

   for (i = 0; i < Size; i++)
   {
      Trial[i] = Board[i];
   }

   Vmirror (Trial, Size);
   i = intncmp (Board, Trial, Size);
   if (i > 0)
   {
      return 0;
   }

   if ( Nequiv > 1 )
   {
      Rotate (Trial, Scratch, Size, -1);
      i = intncmp (Board, Trial, Size);
      if (i > 0)
      {
         return 0;
      }

      if ( Nequiv > 2 )
      {
         Rotate (Trial, Scratch, Size, -1);
         i = intncmp (Board, Trial, Size);
         if (i > 0)
         {
            return 0;
         }

         Rotate (Trial, Scratch, Size, -1);
         i = intncmp (Board, Trial, Size);
         if (i > 0)
         {
            return 0;
         }
      }
   }
   return Nequiv * 2;
}


#define Valid(Board, Size, Row, Diag, AntiD) \
!( Diag[Row-Board[Row]+Size-1] || AntiD[Row+Board[Row]] )

/* Process the partial (or complete) board for the indicated Row */
void Nqueens (int Board[], int Trial[], int Size, int Row)
{
   int Lim, Vtemp, i;

   /* Check for a partial board. */
   if (Row < Size-1)
   {
         if (Valid (Board, Size, Row, Diag, AntiD))
         {
               Mark (Row, Board[Row], Size, Diag, AntiD, 1);
               Nqueens (Board, Trial, Size, Row+1);
               Mark (Row, Board[Row], Size, Diag, AntiD, 0);
         }
         
         Lim = Row ? Size : (Size+1)/2 ;
         for (i = Row+1; i < Lim; i++)
         {
               Vtemp = Board[i];
               Board[i] = Board[Row];
               Board[Row] = Vtemp;
               if (Valid (Board, Size, Row, Diag, AntiD))
               {
                     Mark (Row, Board[Row], Size, Diag, AntiD, 1);
                     Nqueens (Board, Trial, Size, Row+1);
                     Mark (Row, Board[Row], Size, Diag, AntiD, 0);
               }
         }
         
         // Regenerate original vector from Row to Size-1:
         Vtemp = Board[Row];
         for (i = Row+1; i < Size; i++)
         {
               Board[i-1] = Board[i];
         }
         Board[i-1] = Vtemp;
   }
   else // This is a complete board.  Do the required checks
   {
         if ( !Valid (Board, Size, Row, Diag, AntiD) )
            return;
         int tmp = SymmetryOps (Board, Trial, Size);
         if (tmp)
         {
            total_count += tmp;
         }
   }
   return;
}

// Function for worker processes torecieve data for computation and return their calculation of possible nqueens to the master process
void worker(int myPos)
{
      int  nCells = 0, size, k, col, buffer[2];
      int *board = NULL, *trial = NULL;   // Allow for realloc use

      MPI_Status Status;

      MPI_Recv(buffer, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &Status);

      size = buffer[0];
      col  = buffer[1];
      // As long as a valid problem is in hand, do the processing.
      // The server sends a size of zero as a termination message
      while (size > 0)
      {
            total_count  = 0;
            int curr_count;

            // Generate the arrays
            if( size > nCells )
            {  
                  board = (int*) calloc (size, sizeof *board);
                  trial = (int*) calloc (size*2, sizeof *trial);
                  
                  Diag  = (short*) calloc (2*(size-1), sizeof *Diag);
                  AntiD = (short*) calloc (2*(size-1), sizeof *AntiD);

                  for (int i = 0; i < size; i++)
                  {
                        board[i] = i;
                  }
                  nCells = size;
            }
            swap (board[0], board[col]);

            Mark (0, board[0], size, Diag, AntiD, 1);

            Nqueens (board, trial, size, 1);
            Mark (0, board[0], size, Diag, AntiD, 0);
            swap (board[0], board[col]);     //Backtrack

            curr_count = total_count;

            MPI_Send(&curr_count, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

            MPI_Recv(buffer, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &Status);
            size = buffer[0];
            col  = buffer[1];
      }
      // Recieving Exit Message
      MPI_Recv(buffer, 0, MPI_INT, 0, 4, MPI_COMM_WORLD, &Status);
      return;
}

int main(int argc, char *argv[])
{
      int size, rank;
      double t1,t2;

      MPI_Init(&argc, &argv);
      MPI_Comm_rank (MPI_COMM_WORLD, &rank);
      MPI_Comm_size (MPI_COMM_WORLD, &size);

      if(size == 1)
      {
         cout << "Error: Please input more than one processors as it is a master worker algorithm" << endl;
         return 0;
      }

      if(argc != 2) 
      {
         if(rank == 0)
            cout << "Wrong input format" << endl;
         return 0;
      }

      int n = atoi(argv[1]);
      if ( rank == 0 )    // Master Process
      {
         t1 = record_time();
         master(n);
         t2 = record_time();
      }
      else // Worker Processes
      {
         worker(rank);
      }

      if(rank == 0)
      {
            //cout << total_count << endl;
            cout << (t2-t1) << endl;
      }
      MPI_Finalize();
      return 0;
}
