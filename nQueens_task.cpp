#include <vector>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <queue>
using namespace std;

class Node {
public:
  int row=0;
  vector<bool> cols, diag, antiDiag;
  Node (int n){
    // position in column
    cols.resize(n, false);

    // position of the queen in the diagonal way
    diag.resize(2 * n, false);

    // position of a queen in the anit diagonal
    antiDiag.resize(2 * n, false);
  }

  void placeQueen(int row, int col, int curDiag, int curAnti){
    // place the queen -> change the correspoding position
    this -> cols[col] = true;
    this -> diag[curDiag] = true;
    this -> antiDiag[curAnti] = true;
    this -> row = row;
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

  void totalNQueens(queue<Node *> Q, int numQueens, int numThreads, int *num){
    distributeTask(Q, numQueens, numThreads, num);
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
      node -> placeQueen(row + 1, col, curDiag, curAnti);
      backtracking(n, row + 1, node, num);
      node -> removeQueen(col, curDiag, curAnti);
    }
  }
  
  void distributeTask(queue<Node *> taskQueue, int numQueens, int numThreads, int *num){
    /*
      distribute the tasks across the thread
    */
    bool done = false;
    #pragma omp parallel num_threads(numThreads)
    while (true){
      Node *curTask;
      // let one thread take a task in a queue one by one
      // need to concern about the race condition
      #pragma omp critical
      {
        if (taskQueue.empty())
          done = true;
        else {
          curTask = taskQueue.front();
          taskQueue.pop();
        }
      }

      //no more task
      if (done)
        break;
      // run subtask in parallel
      backtracking(numQueens, curTask -> row + 1, curTask, num);
      delete curTask;
    }
  }
};

int main(int argc, char **argv){
  nQueens *sol = new nQueens();
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
  /*
    create task queue to be at least the number of threads.
    we should explore more on creating small tasks -> better load balancing.
    large tasks -> less communication
  */
  queue<Node *> taskQueue;
  for (int i=0; i<max(numThreads, numQueens); i++){
    int row = i / numQueens;
    if (row > 0){
      // create smaller tasks in the case that number of threads is 
      // higher than the board size
      Node *prev = taskQueue.front();

      // pop the first task out
      taskQueue.pop();
      for (int col=0; col<numQueens; col++){
        int curDiag = row - col + numQueens;
        int curAnti = row + col;
        if (prev -> cols[col] || 
            prev -> diag[curDiag] || 
            prev -> antiDiag[curAnti])
          continue;
        Node *node = new Node(numQueens);
        node -> cols = prev -> cols;
        node -> diag = prev -> diag;
        node -> antiDiag = prev -> antiDiag;
        
        node -> placeQueen(row, col, curDiag, curAnti);
        
        taskQueue.push(node);
        i++;
      }
      delete prev; // Free memory for the previous task
    }
    else {
      int col = i % numQueens;
      Node *node = new Node(numQueens);
      int curDiag = row - col + numQueens;
      int curAnti = row + col;
      node -> placeQueen(row, col, curDiag, curAnti);
      // place a task on the queue
      taskQueue.push(node);
    }
  }
  int num = 0;
  sol -> totalNQueens(taskQueue, numQueens, numThreads, &num);
  cout << "The total number of solutions to the n-queens puzzle: " <<num <<endl;
  delete sol;
  return 0;
}