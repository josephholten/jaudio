CC = gcc
FLAGS = -g -Wall -Werror -Wextra

all: test.out

wav.o: wav.c
	$(CC) $(FLAGS) -c wav.c

test.out: test.c wav.o
	$(CC) $(FLAGS) test.c wav.o -o test.out
