#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
using namespace std;

class nQueens {
public:
  //constructor
  nQueens(){

  }

  int totalNQueens(int n) {
    vector<bool> cols(n, false);
    vector<bool> diag(2*n, false);
    vector<bool> antiDiag(2*n, false);
    return backtracking(0, cols, diag, antiDiag, n);
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
  int numQueens=0;
  int opt;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      numQueens = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: ./nQueens [-n number of queens]\n");
      exit(EXIT_FAILURE);
    }
  }
  cout << sol -> totalNQueens(numQueens) << endl;
  delete sol;
  return 0;
}