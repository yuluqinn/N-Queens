#include <vector>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <math.h>
#include <queue>
using namespace std;

#define MAX_GRID 32

class Node {
public:
  bool cols[MAX_GRID] = {};
  bool diag[2 * MAX_GRID] = {};
  bool antiDiag[2 * MAX_GRID] = {}; 
  Node (Node *prev = nullptr){
    if (prev) {
      copy(prev->cols, prev->cols + MAX_GRID, cols);
      copy(prev->diag, prev->diag + 2 * MAX_GRID, diag);
      copy(prev->antiDiag, prev->antiDiag + 2 * MAX_GRID, antiDiag);
    } 
  }

  void placeQueen(int col, int curDiag, int curAnti){
    // place the queen -> change the correspoding position
    this -> cols[col] = true;
    this -> diag[curDiag] = true;
    this -> antiDiag[curAnti] = true;
  }

  void removeQueen(int col, int curDiag, int curAnti){
    // remove queen from the board
    this -> cols[col] = false;
    this -> diag[curDiag] = false;
    this -> antiDiag[curAnti] = false;
  }
};

class nQueens {
public:
  //constructor
  nQueens(){

  }

  void totalNQueens(int numQueens, int numThreads, int numChild, int *num){
    distributeTask(numQueens, numThreads, numChild, num);
  }
private:
  void backtracking(int n, int row, Node *node, int *num){
    if (row == n){
      // add the number of solution
      // Beaware! can't use atomic -> 2 instructions.
      #pragma omp critical
      {
        (*num)++;
      }
      
      return;
    }
    
    for (int col=0; col<n; col++){
      int curDiag = row - col + n;
      int curAnti = row + col;
      
      //check if we can place a queen
      if (node -> cols[col] || 
          node -> diag[curDiag] || 
          node -> antiDiag[curAnti])
        continue;
      node -> placeQueen(col, curDiag, curAnti);
      backtracking(n, row + 1, node, num);
      node -> removeQueen(col, curDiag, curAnti);
    }
  }
  
  void distributeTask(int numQueens, int numThreads, int numChild, int *num){
    /*
      distribute the tasks across the thread
    */
    #pragma omp parallel num_threads(numThreads) 
    {
      #pragma omp single nowait
      {
        
        createTask(0, numChild, numQueens, numThreads, nullptr, num);
      }
    }
  }

  void createTask(int row, int n, int numQueens, int numThreads, Node *prev, int *num){
    if (row == n){
      #pragma omp task
      backtracking(numQueens, row, prev, num);
      return;
    }
    for (int col=0; col<numQueens; col++){
      int curDiag = row - col + numQueens;
      int curAnti = row + col;
      if (prev && (prev -> cols[col] || 
              prev -> diag[curDiag] || 
              prev -> antiDiag[curAnti]))
        continue;
      Node *node = new Node(prev);
      node -> placeQueen(col, curDiag, curAnti);
      createTask(row + 1, n, numQueens, numThreads,
              node, num);
    }
  }
};

int main(int argc, char **argv){
  nQueens *sol = new nQueens();
  int numQueens=1, numThreads=1;
  int numChild=1;
  int opt;
  while ((opt = getopt(argc, argv, "n:t:k:")) != -1) {
    switch (opt) {
    case 'n':
      numQueens = atoi(optarg);
      break;
    case 't':
      numThreads = atoi(optarg);
      break;
    case 'k':
      numChild = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: ./nQueens [-n number of queens] \
                                [-t number of threads] \
                                [-k number of children]\n");
      exit(EXIT_FAILURE);
    }
  }
  /*
    create task queue to be at least the number of threads.
    we should explore more on creating small tasks -> better load balancing.
    large tasks -> less communication
  */
 
  int num = 0;
  sol -> totalNQueens(numQueens, numThreads, numChild, &num);
  cout << "The total number of solutions to the n-queens puzzle: " <<num <<endl;
  delete sol;
  return 0;
}
