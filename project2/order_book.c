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

void clearStr(char *string);

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
				char side;
				char symbol[MAX_SYMBOL_LENGTH];
				int id, quantity;
				double price;
				char transaction;

				if (foundInput) {
						FILE *file = fopen(inputFile,"r");
						if (file == NULL) {
								printf("Could not Open file\n");
								return 1;
						}
						while((transaction = fgetc(file)) != EOF) {
								if (transaction == 'A') {
										fscanf(file, " %d %c %s %d %lf", &id, &side, symbol, &quantity, &price);
								}
								else if (transaction == 'X') {
										fscanf(file, " %d %s", &id, symbol);
								}
								else if (transaction == 'T') {
										fscanf(file, " %d %s %d", &id, symbol, &quantity);

								}	
								else if (transaction == 'C') {
										fscanf(file, " %d %s %d", &id, symbol, &quantity);

								}	
								else if (transaction == 'R') {
										fscanf(file, " %d %s %d %lf", &id, symbol, &quantity, &price);

								}
								else if (transaction == '\n') {
										continue;
								}
								else {
										printf("Invalid transaction\n");
								}
								clearStr(symbol);
						}
				}

				else {
					while((transaction = getchar()) != EOF) {
								if (transaction == 'A') {
										scanf(file, " %d %c %s %d %lf", &id, &side, symbol, &quantity, &price);
								}
								else if (transaction == 'X') {
										scanf(file, " %d %s", &id, symbol);
								}
								else if (transaction == 'T') {
										scanf(file, " %d %s %d", &id, symbol, &quantity);

								}	
								else if (transaction == 'C') {
										scanf(file, " %d %s %d", &id, symbol, &quantity);

								}	
								else if (transaction == 'R') {
										scanf(file, " %d %s %d %lf", &id, symbol, &quantity, &price);

								}
								else if (transaction == '\n') {
										continue;
								}
								else {
										printf("Invalid transaction\n");
								}
								clearStr(symbol);
					}
				}
		}
}

int myHash(int id) {
		return (id % HASHTABLESIZE);
}

void clearStr(char *string) {
		int i = 0;
		for(i; i < strlen(string); i++) {
				string[i] = '\0';
		}
}
