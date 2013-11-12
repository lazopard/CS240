CC=gcc

CFLAGS=-Wall -m32 -std=c89 -g -o backup

all: backup

backup: backup.c backup.h
	$(CC) $(CFLAGS) backup.c

clean:
	rm backup
