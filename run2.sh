#!/bin/sh
g++ -fopenmp -o nQueens_task nQueens_task.cpp

for t in 1 2 4 8 16
do
  echo "=====================nQueens_task===$t====================================="
  time ./nQueens_task -n 17 -t $t -k 2
done