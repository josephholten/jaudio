CC = gcc
FLAGS = -g -Wall -Werror -Wextra

wav: wav.c
	$(CC) $(FLAGS) wav.c -o wav
