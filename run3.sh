#!/bin/sh
g++ -fopenmp -o nQueens_task nQueens_task.cpp

for k in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
do
  echo "=====================nQueens_task===$k====================================="
  time ./nQueens_task -n 16 -t 4 -k $k
done