#include <vector>
#include <iostream>
#include <unistd.h>
#include <omp.h>
using namespace std;

class nQueens {
public:
  //constructor
  nQueens(){

  }

  int totalNQueens(int n, int numThreads) {
  
    int ans=0;
   
    
    #pragma omp parallel for num_threads(numThreads) \
      reduction(+:ans)
    
      for (int col=0; col<n; col++){
        vector<bool> cols(n, false);
        vector<bool> diag(2*n, false);
        vector<bool> antiDiag(2*n, false);
        int curDiag = - col + n;
        int curAnti = col;
        cols[col] = true;
        diag[curDiag] = true;
        antiDiag[curAnti] = true;
        ans += backtracking(1, cols, diag, antiDiag, n);
      }
    
    
    return ans;
    
    
  }

private:
  int backtracking(int row,
                   vector<bool>& cols,
                   vector<bool>& diag,
                   vector<bool>& antiDiag,
                   int n){
    if (row == n)
      return 1;
    int ans=0;
   
    for (int col=0; col<n; col++){
      int curDiag = row - col + n;
      int curAnti = row + col;
      if (cols[col] || diag[curDiag] || antiDiag[curAnti])
        continue;
      cols[col] = true;
      diag[curDiag] = true;
      antiDiag[curAnti] = true;
      ans += backtracking(row + 1, cols, diag, antiDiag, n);
      cols[col] = false;
      diag[curDiag] = false;
      antiDiag[curAnti] = false;
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
