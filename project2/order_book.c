/**
 * order_book.c
 *
 * The main program to load input, manipulate internal storage, and generate output.
 */
#define HASHTABLESIZE 100
#define INPUT "-i"
#define OUTPUT "-o"
#define ASCENDING "-asc"
#define HASHTABLE "-h"
#define DEFAULTHASH "-dh"
#define HASH 13
#define LLIST 21

#include <stdio.h>
#include "list.h"
#include "hash.h"
#include "default_hash.h"
#include "string.h"

int main(int argc, char **argv) {
  int i, foundInput, foundOutput, foundHashtable, foundDefault, storageType;
  foundInput=foundOutput=foundHashtable=foundDefault=storageType = 0;
  char *inputFile, *outputFile, *inFile;
  inputFile = outputFile = inFile = NULL;
  for(i=1;i < argc; i++) {
  	if (!strcmp(argv[i], INPUT)) {
		foundInput++;
		inputFile = argv[i+1];
		i++;
	}
	if (!strcmp(argv[i], OUTPUT)) {
		foundOutput++;
		inputFile = argv[i + i];
		i++;
	}
	if (!strcmp(argv[i], HASHTABLE)) {
		foundHashtable++;
		storageType = HASH;
	}
	if (!strcmp(argv[i], DEFAULTHASH)) {
		foundDefault++;
		storageType = HASH;
	}
  }
  if (foundHashtable && foundDefault) {
  	printf("Invalid Argument usage.\n");
	return 1;
  }
  else if  (!(foundHashtable || foundDefault)) {
  	storageType = LLIST;
  }

  else {
  	if (foundInput) {
		FILE *file = fopen(inputFile,"r");
		if (file == NULL) {
			printf("Could not Open file\n");
			return 1;
		}
	}
	else {

	}
  }
}

int myHash(int id) {
	return (id % HASHTABLESIZE);
}
