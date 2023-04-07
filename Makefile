CC=g++
CFLAGS=-pedantic -fsigned-char -Wall -Werror -Wextra -fopenmp

.PHONY: all
all: nQueens_task

nQueens_task: nQueens_task.o
	$(CC) $(CFLAGS) -o nQueens_task nQueens_task.o

nQueens_task.o: nQueens_task.cpp
	$(CC) $(CFLAGS) -c nQueens_task.cpp

.PHONY: clean
clean:
	rm -f *.o nQueens_task
