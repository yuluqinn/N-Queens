#include <iostream>
#include <cstdlib>
#include <chrono>
#include <unistd.h>

using namespace std;

int solutions = 0;


void setQueen(int positions[], int row, int col, int size) {
    for(int i = 0; i < row; i++) {
        // Check if position is safe
        if (positions[i] == col || abs(positions[i] - col) == (row - i)) {
            return;
        }
    }
    // col is ok, set queen.
    positions[row] = col;
    if(row == size - 1) {
        solutions++;
    } else {
        for(int i = 0; i < size; i++) {
            setQueen(positions, row + 1, i, size);
        }
    }
}

int main(int argc, char **argv) {
  int opt;
  int size=1;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      size = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: ./nQueens [-n number of queens] [-t number of threads]\n");
      exit(EXIT_FAILURE);
    }
  }
  
  int positions[size];


  for(int i = 0; i < size; i++) {
      setQueen(positions, 0, i, size);
  }

    cout << "Number of solutions: " << solutions << endl;
   
    return 0;
}
