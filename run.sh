#!/bin/sh
g++ -fopenmp -o baseline baseline.cpp
g++ -fopenmp -o nQueens nQueens.cpp
g++ -fopenmp -o nQueens_task nQueens_task.cpp
thread=4
for i in {12..16}
do
  echo "=====================baseline===$i====================================="
  time ./baseline -n $i
done

echo "*********************************************************************"
echo "*********************************************************************"

for i in {12..17}
do
  echo "=====================nQueens===$i====================================="
  time ./nQueens -n $i -t thread
done

for i in {12..17}
do
  echo "=====================nQueens_task===$i====================================="
  time ./nQueens_task -n $i -t thread -k 2
done