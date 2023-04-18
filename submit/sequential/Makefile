CC=g++
CFLAGS=-pedantic -fsigned-char -Wall -Werror -Wextra -fopenmp

.PHONY: all
all: nQueens

nQueens_task: nQueens.o
	$(CC) $(CFLAGS) -o nQueens nQueens.o

nQueens_task.o: nQueens.cpp
	$(CC) $(CFLAGS) -c nQueens.cpp

.PHONY: clean
clean:
	rm -f *.o nQueens