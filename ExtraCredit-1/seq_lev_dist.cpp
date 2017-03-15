#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <omp.h>
#include <iostream>
#include <sys/time.h>

using namespace std;

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))


double record_time()
{
    timeval tim;
    
    gettimeofday(&tim, NULL);
    double t0 = (double)tim.tv_sec+ (double)(tim.tv_usec/1000000.0); // current time in seconds

    return t0;
}

void deallocate2D(int **arr2D, int rows){
	int i;

	for(i = 0; i < rows; i++)
		free(arr2D[i]);
	free(arr2D);
}

int serial_levenshtein(char *s1, char *s2) 
{
    int i, x, y, s1len, s2len;
    s1len = strlen(s1);
    s2len = strlen(s2);

	int **matrix = new int *[s2len + 1];
	for(i = 0; i < s2len + 1; i++)
	{
		matrix[i] = new int[s1len + 1];
	}

    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
    {
    	matrix[x][0] = matrix[x-1][0] + 1;
    }	
    for (y = 1; y <= s1len; y++)
    {
    	matrix[0][y] = matrix[0][y-1] + 1;
    }
    for (x = 1; x <= s2len; x++)
    {
    	for (y = 1; y <= s1len; y++)
    	{
    		matrix[x][y] = MIN3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));
    	}
    }
    int return_val = matrix[s2len][s1len];
	deallocate2D(matrix, s2len + 1);
	return return_val;
}

int main(int argc, char **argv)
{
	int len = atoi(argv[1]);
	char s1[len];
	char s2[len];

	cin >> s1;
	cin >> s2;
	double t1 = record_time();
	int distance = serial_levenshtein(s1, s2);
	double t2 = record_time();
	//printf("Levenshtein Distance is (%d)\n",distance);
	cout << (t2-t1) << endl;
	return 0;
}