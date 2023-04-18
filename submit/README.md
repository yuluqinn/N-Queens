# Parallelize N-Queens

The code is structured as follows:
```
├── parallel
│   ├── Makefile
│   └── nQueens.cpp
└── sequential
    ├── Makefile
    └── nQueens.cpp
```
## Parallel code
To use our proposed code, navigate to the 'parallel' directory where the code is located, and compile it using the provided Makefile.
```
cd parallel
make
```
### Usage
```
Usage: ./nQueens [-n number of queens] [-t number of threads] [-k number of queens allocated for each task]
```
For example, to run the parallel code for solving the N-Queens problem with a board size of 14, execute the command with 4 threads and a task allocation of 2 queens per task. We recommend using k=2 for big size problem (n >= 12) and k = 1 for small size.
```
./nQueens -n 14 -t 4 -k 2
```

## Sequential code
To use our sequential code, navigate to the 'sequential' directory where the code is located, and compile it using the provided Makefile.
```
cd sequential
make
```
### Usage
```
Usage: ./nQueens [-n number of queens]
```
For example, to run the parallel code for solving the N-Queens problem with a board size of 14.
```
./nQueens -n 14
```