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
    cols.resize(n, false);
    diag.resize(2 * n, false);
    antiDiag.resize(2 * n, false);
  }

  void placeQueen(int row, int col, int curDiag, int curAnti){
    this -> cols[col] = true;
    this -> diag[curDiag] = true;
    this -> antiDiag[curAnti] = true;
    this -> row = row;
  }

  void removeQueen(int col, int curDiag, int curAnti){
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

  int totalNQueens(queue<Node *> Q, int numQueens, int numThreads){
    return distributeTask(Q, numQueens, numThreads);
  }
private:
  int backtracking(int n, int row, Node *node){
    if (row == n)
      return 1;
    int ans=0;
    for (int col=0; col<n; col++){
      int curDiag = row - col + n;
      int curAnti = row + col;
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
  
  int distributeTask(queue<Node *> taskQueue, int numQueens, int numThreads){
    int ans=0;
  
    bool done = false;
    #pragma omp parallel num_threads(numThreads) \
      reduction(+:ans) 
    while (true){
      Node *curTask;
      #pragma omp critical
      {
        if (taskQueue.empty())
          done = true;
        else {
          curTask = taskQueue.front();
          taskQueue.pop();
        }
      }
      
      if (done)
        break;
      
      ans += backtracking(numQueens, curTask -> row + 1, curTask);
      free(curTask);
    }
    return ans;
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
  
  //Node **taskQueue = new Node*[max(numThreads, numQueens)];
  queue<Node *> taskQueue;
  /*
  for (int col=0; col< numQueens; col++){
    Node *node = new Node(numQueens);
    int curDiag = - col + numQueens;
    int curAnti = col;
    node -> cols[col] = true;
    node -> diag[curDiag] = true;
    node -> antiDiag[curAnti] = true;
    node -> row = 0;
    //taskQueue[col] = node;
    taskQueue.push(node);
  }
  */
  for (int i=0; i<max(numThreads, numQueens); i++){
    int row = i / numQueens;
    int col = i % numQueens;
    if (row > 0){
      Node *prev = taskQueue.front();
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
      Node *node = new Node(numQueens);
      int curDiag = row - col + numQueens;
      int curAnti = row + col;
      node -> placeQueen(row, col, curDiag, curAnti);
      taskQueue.push(node);
    }
  }
  exit(0);
  cout << sol -> totalNQueens(taskQueue, numQueens, numThreads) << endl;
  delete sol;
  return 0;
}
