CC = gcc
FLAGS = -g -Wall -Werror -Wextra

all: test_square.out test_empty.out

wav.o: wav.c
	$(CC) $(FLAGS) -c wav.c

test_empty.out: test_empty.c wav.o
	$(CC) $(FLAGS) test_empty.c wav.o -o test_empty.out

test_square.out: test_square.c wav.o
	$(CC) $(FLAGS) test_square.c wav.o -o test_square.out -lm

clean:
	rm *.wav *.out *.o
