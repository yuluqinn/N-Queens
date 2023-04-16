#!/bin/sh
g++ -fopenmp -o par paper_parallel.cpp
g++ -fopenmp -o seq sequential.cpp
thread=4
for i in {12..17}
do
  echo "=====================baseline===$i====================================="
  time ./seq -n $i
done

echo "*********************************************************************"
echo "*********************************************************************"

for i in {12..17}
do
  echo "=====================nQueens===$i====================================="
  time ./par -n $i -t $thread
done

