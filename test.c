/* solving the N-Queens problem using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o nqueens-openmp nqueens-openmp.c
     ./nqueens-openmp n numWorkers

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include <omp.h>

#define MAX_N 16

int check_acceptable(int queen_rows[MAX_N], int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = i+1; j < n; j++)
		{
			// two queens in the same row => not a solution!
			if (queen_rows[i] == queen_rows[j]) return 0;
			
			// two queens in the same diagonal => not a solution!
			if (queen_rows[i] - queen_rows[j] == i - j ||
			    queen_rows[i] - queen_rows[j] == j - i)
			    return 0;
		}
	}

	return 1;
}

int main(int argc, char* argv[]){
  int n;
  long long max_iter = 1;
  
  double start_time, end_time;
  int number_solutions = 0;
        
	{
    int num_workers;
      int i;

      n = (argc > 1) ? atoi(argv[1]) : 8;
      num_workers = (argc > 2) ? atoi(argv[2]) : 30;
      
      omp_set_num_threads(num_workers);
        
      for (i = 0; i < n; i++)
      { 
          max_iter *= (long long)n;
      }
    }
  
	long long iter;
  #pragma omp parallel for \
    reduction(+:number_solutions)
	  for (iter = 0; iter < max_iter; iter++){
      long long code = iter;
      int i;
      int queen_rows[MAX_N] = {0};
      // the index correspond to the queen's number and the queen's collumn
      // we only generate configurations where there's only one queen per collumn
      for (i = 0; i < n; i++){
        queen_rows[i] = code % n;
        code /= n;
      }
      /*
      if (iter == max_iter - 1){
        printf("%d ", max_iter);
        for (int i=0; i<n; i++)
          printf("%d ", queen_rows[i]);
        exit(0);
      }
      */
      
      if (check_acceptable(queen_rows, n)){
       
        number_solutions++;
        /*
        #pragma omp critical
        {
          printf("\n");
          for (i = 0; i < n; i++)
          {
              int j;
            for (j = 0; j < n; j++)
            {
              if (queen_rows[i] == j)	printf("|X");
              else printf("| ");
            }
            printf("|\n");
          }
          printf("\n");
        }
        */
        
      }
	  }
  printf("%d ", number_solutions);
   
	return 0;
}
