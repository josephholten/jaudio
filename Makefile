CC = gcc
FLAGS = -g -Wall -Werror -Wextra

all: test.out test_empty.out

wav.o: wav.c
	$(CC) $(FLAGS) -c wav.c

test.out: test.c wav.o
	$(CC) $(FLAGS) test.c wav.o -o test.out

test_empty.out: test_empty.c wav.o
	$(CC) $(FLAGS) test_empty.c wav.o -o test_empty.out
