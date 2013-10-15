CC = gcc
CFLAGS = -std=c99 -m32

all: orderbook

orderbook: order_book.c libhash.a liblist.a
	 $(CC) $(CFLAGS) -o orderbook order_book.c -L. -llist -lhash

libhash.a: hash.o
	ar rcu libhash.a hash.o

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c

liblist.a: list.o
	ar rcu liblist.a list.o

list.o: list.c
	$(CC) $(CFLAGS) -c list.c

clean:
	rm orderbook
