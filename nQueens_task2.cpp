#include <vector>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <math.h>
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

  int totalNQueens(queue<Node *>& Q, int numQueens, int numThreads){
    return distributeTask(Q, numQueens, numThreads);
  }
private:
  int backtracking(int n, int row, Node *node){
    if (row == n){
      // add the number of solution
      // Beaware! can't use atomic -> 2 instructions.
      return 1;
    }
    int ans=0;
    for (int col=0; col<n; col++){
      int curDiag = row - col + n;
      int curAnti = row + col;

      //check if we can place a queen
      if (node -> cols[col] || 
          node -> diag[curDiag] || 
          node -> antiDiag[curAnti])
        continue;
      node -> placeQueen(row + 1, col, curDiag, curAnti);
      ans += backtracking(n, row + 1, node);
      node -> removeQueen(col, curDiag, curAnti);
    }
    return ans;
  }
  
  int distributeTask(queue<Node *>& taskQueue, int numQueens, int numThreads){
    /*
      distribute the tasks across the thread
    */
    bool done = false;
    int num=0;
    #pragma omp parallel num_threads(numThreads) reduction(+:num)
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
      num += backtracking(numQueens, curTask -> row + 1, curTask);
      delete curTask;
    }
    return num;
  }
};

int main(int argc, char **argv){
  nQueens *sol = new nQueens();
  int numQueens=1, numThreads=1;
  bool largeTasks = true;
  int opt;
  while ((opt = getopt(argc, argv, "n:t:k")) != -1) {
    switch (opt) {
    case 'n':
      numQueens = atoi(optarg);
      break;
    case 't':
      numThreads = atoi(optarg);
      break;
    case 'k':
      largeTasks = false;
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
  //int n= numThreads < numQueens ? numQueens : pow(numQueens, 2) + numQueens;
  int n= largeTasks ? numQueens : pow(numQueens, 2) + numQueens;
  for (int i=0; i<n; i++){
    //int row = (int)log(i) / log(numQueens);
    int row = i < numQueens ? 0 : 1;
    if (row > 0){
      // create smaller tasks in the case that number of threads is 
      // higher than the board size
      Node *prev = taskQueue.front();

      // pop the first task out
      taskQueue.pop();
      for (int col=0; col<numQueens; col++, i++){
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
        //cout << row <<", "<<col<<endl;
        taskQueue.push(node);
        
      }
      //cout << endl;
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
  //exit(0);
  //cout << taskQueue.size() ; exit(0);
  int num = sol -> totalNQueens(taskQueue, numQueens, numThreads);
  cout << "The total number of solutions to the n-queens puzzle: " <<num <<endl;
  delete sol;
  return 0;
}
