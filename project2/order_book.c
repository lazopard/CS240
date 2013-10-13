/**
 * order_book.c
 *
 * The main program to load input, manipulate internal storage, and generate output.
 */

#define INPUT "-i"
#define OUTPUT "-o"
#define ASCENDING "-asc"
#define HASHTABLE "-h"
#define DEFAULTHASH "-dh"
#define HASH 13
#define LIST 21

#include <stdio.h>
#include "list.h"
#include "hash.h"
#include "default_hash.h"

/**
 * TODO: Feel free to create whatever functions you need.
 */

int main(int argc, char **argv) {
  int i, foundInput, foundOutput, foundHashtable, foundDefault, storageType;
  foundInput=foundOutput=foundHashtable=foundDefault=storageType = 0;
  char *inputFile, *outputFile, *inFile;
  *inputFile = *outputFile = *inFile = NULL;
  for(i=1;i , argc; i++) {
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
  }
  else if (!(foundHastable || foundDefault)) {
  	storageType = LLIST;
  }
  else {
  	
  }
  return 0;
}

/*

int main(int argc, char **argv) {
	if (argumentsAreValid(foundInput,foundOutput,foundEncription,foundDecription)) {
		if (!(inFile = readFile(inputFile, &size))) {
			printf("Error loading input file.\n");
			return 1;
		}

		else if (foundEncription) {
			if (option == SHIFT) {
				char encriptionOutput[size];
				shift_enc(inFile,shiftN,encriptionOutput);
				printFile(outputFile,DISCARD_CONTENT,encriptionOutput);
			}
			else if (option == SUBSTITUTION) {
				char encriptionOutput[size];
				subst_enc(inFile, ALPHABET, SUBSTITUTION_KEY, encriptionOutput);
				printFile(outputFile,DISCARD_CONTENT,encriptionOutput);
			}
			else {
			}
		}

		else {
			if (option == SHIFT) {
				char decriptionOutput[size];
				shift_dec(inFile,shiftN,decriptionOutput);
				printFile(outputFile,DISCARD_CONTENT,decriptionOutput);
			}	
			else if (option == BRUTE_FORCE) {
					shift_bf(inFile, outputFile);
			}
			else if (option == SUBSTITUTION) {
				char decriptionOutput[size];
				subst_dec(inFile, ALPHABET, SUBSTITUTION_KEY, decriptionOutput); 
				printFile(outputFile,DISCARD_CONTENT,decriptionOutput);
			}
			else if (option == FREQUENCY) {
				analyzeFreq(inFile, outputFile);
			}
			else {
			}
		}

		freeMem();
		return 0;
	}
}
*/
