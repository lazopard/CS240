CC=gcc
CFLAGS=-Wall -m32 -std=gnu89 -g -o myTerminal
all: myTerminal

myTerminal: myTerminal.c
	$(CC) $(CFLAGS) myTerminal.c libmyTerminal_ref.a
clean:
	rm myTerminal
