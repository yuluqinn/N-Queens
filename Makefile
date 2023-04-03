CC=g++
CFLAGS=-g -pedantic -fsigned-char -Wall -Werror -Wextra -fopenmp

.PHONY: all
all: nQueens

nQueens: nQueens.o
	$(CC) $(CFLAGS) -o nQueens nQueens.o

nQueens.o: nQueens.cpp
	$(CC) $(CFLAGS) -c nQueens.cpp

.PHONY: clean
clean:
	rm -f *.o nQueens
