#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;

int solutions = 0;
int size;

void setQueen(int positions[], int row, int col) {
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
            setQueen(positions, row + 1, i);
        }
    }
}

int main() {
    cout << "Enter the size of the chessboard: ";
    cin >> size;

    int positions[size];

    // Start timer
    auto start_time = chrono::high_resolution_clock::now();

    for(int i = 0; i < size; i++) {
        setQueen(positions, 0, i);
    }

    // End timer
    auto end_time = chrono::high_resolution_clock::now();

    // Calculate duration
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << "Number of solutions: " << solutions << endl;
    cout << "Time taken: " << float(duration/float(1000)) << " seconds" << endl;

    return 0;
}
