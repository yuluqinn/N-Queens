#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <unistd.h>
#include <chrono>

using namespace std;

int solutions = 0;
int n;

void setQueen(int positions[], int row, int col) {
    for(int i=0; i<row; i++) {
        // Check if position is safe
        if (positions[i]==col || abs(positions[i]-col) == (row-i)) {
            return;
        }
    }
    // col is ok, set queen.
    positions[row]=col;
    if(row==n-1) {
        #pragma omp atomic
        solutions++;
    } else {
        for(int i=0; i<n; i++) {
            setQueen(positions, row+1, i);
        }
    }
}

int main(int argc, char **argv) {
  int num_threads=1;
  int opt;
  while ((opt = getopt(argc, argv, "n:t:")) != -1) {
    switch (opt) {
    case 'n':
      n = atoi(optarg);
      break;
    case 't':
      num_threads = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: ./nQueens [-n number of queens] [-t number of threads]\n");
      exit(EXIT_FAILURE);
    }
  }
    omp_set_num_threads(num_threads);
    
    // Start timer
   

    #pragma omp parallel for
    for(int i=0; i<n; i++) {
        setQueen(new int[n], 0, i);
    }

   
    cout << "Number of solutions: " << solutions << endl;
    
    return 0;
}