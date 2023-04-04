#include <vector>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <cstring>
using namespace std;

class nQueens {
public:
  //constructor
  nQueens(){

  }

  int totalNQueens(int n, int numThreads) {
    long long numIter = 1;
    for (int i = 0; i < n; i++)
      numIter *= (long long) n;
    
     int ans=0;
    bool cols[n] = {};
    bool diag[2 * n] = {};
    bool antiDiag[2 * n] = {};
    #pragma omp parallel for num_threads(numThreads)\
      reduction(+:ans) private (cols, diag, antiDiag)
    
    for (long long iter=0; iter<numIter; iter++){
      long long idx = iter;
      memset(cols, false, n);
      memset(diag, false, 2 * n);
      memset(antiDiag, false, 2 * n);
      bool nextRound=false;
      
      for (int row=0; row<n; row++){
        int curCol = idx % n;
        int curDiag = row - curCol + n;
        int curAnti = row + curCol;
        if (cols[curCol] || diag[curDiag] || antiDiag[curAnti]){
          nextRound = true;
          break;
        }
        cols[curCol] = true;
        diag[curDiag] = true;
        antiDiag[curAnti] = true;
        idx /= n;
      }
      if (nextRound)
        continue;
      ans++;
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
  cout << sol -> totalNQueens(numQueens, numThreads) << endl;
  delete sol;
  return 0;
}
