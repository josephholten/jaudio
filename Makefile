CC = gcc
FLAGS = -g -Wall -Werror -Wextra

all: test_synth.out test_empty.out wav_plot.out

util.o: util.c
	$(CC) $(FLAGS) -c util.c

wav.o: wav.c
	$(CC) $(FLAGS) -c wav.c

synth.o: synth.c
	$(CC) $(FLAGS) -c synth.c

test_empty.out: test_empty.c wav.o util.o
	$(CC) $(FLAGS) test_empty.c wav.o util.o -o test_empty.out

test_synth.out: test_synth.c wav.o synth.o util.o
	$(CC) $(FLAGS) test_synth.c wav.o synth.o util.o -o test_synth.out -lm

wav_plot.out: wav_plot.c wav.o synth.o util.o
	$(CC) $(FLAGS) wav_plot.c wav.o synth.o util.o -o wav_plot.out -lm

clean:
	rm *.wav *.out *.o
