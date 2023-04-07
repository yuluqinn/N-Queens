#include <omp.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <getopt.h>

#define MAX_N 16
struct board {
  int pieces;
  int places[MAX_N]; // Queen's position in each row
  struct board *next;
};

bool no_threat(struct board *ptr){
  int row = ptr->pieces - 1;
  int col = ptr->places[row];
  for (int i=0; i<row; i++){
    int pos = abs(ptr->places[i] - col);
    if (pos == 0 || pos == row-i)
      return false;
  }
  return true;
}

int search(int n, struct board *ptr){
  if (ptr -> pieces == n)
    return 1;
  
  int ans=0;
  ptr -> pieces++;
  for (int i=0; i<n; i++){
    ptr -> places[ptr -> pieces - 1] = i;
    if (no_threat(ptr))
      ans += search(n, ptr);
  }
  ptr -> pieces--;
  return ans;
}

int search_for_solutions(int n,
  struct board **stack, int numThreads){
    
  int ans=0;
  #pragma omp parallel num_threads(numThreads) \
    reduction(+:ans)
  {
    struct board *ptr;
    while (*stack){
      #pragma omp critical
      {
        ptr = *stack;
        *stack = (*stack) -> next;
      }
      ans += search(n, ptr);
      free(ptr);
    }
  }
  return ans;
}

int main(int argc, char **argv){
  int numQueens=1, numThreads=1;
  int opt;
  while ((opt = getopt(argc, argv, "n:t:")) != -1) {
    switch (opt) {
    case 'n':
      numQueens = atoi(optarg);
      break;
    case 't':
      numThreads = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: ./nQueens [-n number of queens] [-t number of threads]\n");
      exit(EXIT_FAILURE);
    }
  }
  struct board *stack=NULL;
  struct board *initial;
  for (int i=0; i<numQueens; i++){
    initial = (struct board *)malloc(sizeof(struct board));
    initial -> pieces = 1;
    initial -> places[0] = i;
    initial -> next = stack;
    stack = initial;
  }
  int num_solutions = search_for_solutions(numQueens, &initial, numThreads);
  
  
  printf("%d ", num_solutions);
}