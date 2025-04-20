CC = gcc
FLAGS = -g -Wall -Werror -Wextra

all: test_synth.out test_empty.out

wav.o: wav.c
	$(CC) $(FLAGS) -c wav.c

synth.o: synth.c
	$(CC) $(FLAGS) -c synth.c

test_empty.out: test_empty.c wav.o
	$(CC) $(FLAGS) test_empty.c wav.o -o test_empty.out

test_synth.out: test_synth.c wav.o synth.o
	$(CC) $(FLAGS) test_synth.c wav.o synth.o -o test_synth.out -lm

clean:
	rm *.wav *.out *.o
