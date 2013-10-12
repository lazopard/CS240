CC=gcc
CFLAGS = -g -std=c99 -m32 -o
all: sort

sort: sort.c liblist.a
	$(CC) $(CFLAGS) sort sort.c liblist.a
clean:
	rm sort
