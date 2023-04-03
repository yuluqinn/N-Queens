CC=g++
CFLAGS=-g -pedantic -std=gnu17 -fsigned-char -Wall -Werror -Wextra
LDFLAGS=-fopenmp
.PHONY: all
all: nQueens

nQueens: nQueens.o

nQueens.o: nQueens.cpp

.PHONY: clean
clean:
	rm -f *.o nyuenc