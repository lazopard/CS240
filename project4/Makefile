CC=gcc
CFLAGS=-Wall -m32 -g -fprofile-arcs -ftest-coverage -o

all: pipe_counter shm_counter

pipe_counter: pipe_counter.c
	$(CC) $(CFLAGS) pipe_counter pipe_counter.c

shm_counter: shm_counter.c
	$(CC) $(CFLAGS) shm_counter shm_counter.c
clean:
	rm -rf pipe_counter shm_counter *.gcda *.gcno *.dSYM
