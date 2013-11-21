CC=gcc
CFLAGS=-Wall -m32 -g -fprofile-arcs -ftest-coverage -o

all: myTerminal

myTerminal: myTerminal.c
	$(CC) $(CFLAGS) myTerminal myTerminal.c libmyTerminal_ref.a
clean:
	rm myTerminal
