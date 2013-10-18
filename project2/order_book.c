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
	int i, foundInput, foundOutput, foundHashtable, foundDefault, storageType, foundAscending;
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
			outputFile = argv[i + i];
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
		if (!strcmp(argv[i], ASCENDING)) {
			foundAscending++;
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
		if (foundHashtable) {
			struct hashStorage *hash = createHash(HASHTABLESIZE, &myHash, &printOrder);
		}
		else if (foundDefault) {
			struct hashStorage *hash = createHash(HASHTABLESIZE, &default_hash, &printOrder);
		}
		else {
			struct hashStorage *hash = createhash(HASHTABLESIZE, NULL, &printOrder);
		}
		char side;
		char symbol[MAX_SYMBOL_LENGTH];
		int id, quantity, index;
		double price;
		char transaction;
		struct order newOrder;

		if (foundInput) {
			FILE *inFile = fopen(inputFile,"r");
			if (inFile == NULL) {
				printf("Could not Open file\n");
				return 1;
			}
			while((transaction = fgetc(inFile)) != EOF) {
				if (transaction == 'A') {
					fscanf(inFile, " %d %c %s %d %lf", &id, &side, symbol, &quantity, &price);
					newOrder = {.id = id, .side = side, .symbol = symbol,
						.quantity = quantity, .price = price};
					addOrder(hash, &newOrder);
				}
				else if (transaction == 'X') {
					fscanf(inFile, " %d %s", &id, symbol);
					if (foundHashtable || foundDefault) {
						int index = hash->funcHash(id);
						cancelOrder(hash, *(hash->table + index));
					}
					else {
						cancelOrder(hash, getOrderNode(getHashtable(hash), id));
					}
				}
				else if (transaction == 'T') {
					fscanf(inFile, " %d %s %d", &id, symbol, &quantity);
					newOrder = {.id = id, .symbol = symbol, .side ='a',
						.quantity = quantity, .price = 0.0};
					reduceOrderQuantity(hash, &newOrder);
				}	
				else if (transaction == 'C') {
					fscanf(inFile, " %d %s %d", &id, symbol, &quantity);
					newOrder = {.id = id, .symbol = symbol, .side ='a',
						.quantity = quantity, .price = 0.0};
					reduceOrderQuantity(hash, &newOrder);
				}	
				else if (transaction == 'R') {
					fscanf(inFile, " %d %s %d %lf", &id, symbol, &quantity, &price);
					index = hash->funcHash(id);
					newOrder = {.id = id, .symbol = symbol, .side = 'a',
						.quantity = quantity, .price = price};
					changeOrder(hash, &newOrder);

				}
				else if (transaction == '\n') {
					continue;
				}
				else {
					printf("Invalid transaction\n");
				}
				clearStr(symbol);
				clearStr(newOrder.symbol);
			}
		}

		else {
			while((transaction = getchar()) != EOF) {
				if (transaction == 'A') {
					scanf(inFile, " %d %c %s %d %lf", &id, &side, symbol, &quantity, &price);
					newOrder = {.id = id, .side = side, .symbol = symbol,
						.quantity = quantity, .price = price};
					addOrder(hash, &newOrder);

				}
				else if (transaction == 'X') {
					scanf(inFile, " %d %s", &id, symbol); 
					if (foundHashtable || foundDefault) {
						index = hash->funcHash(id);
						cancelOrder(hash, *(hash->table + index));
					}
					else {
						cancelOrder(hash, getOrderNode(getHashtable(hash), id));
					}		
				}
				else if (transaction == 'T') {
					scanf(inFile, " %d %s %d", &id, symbol, &quantity);
					newOrder = {.id = id, .symbol = symbol, .side ='a',
						.quantity = quantity, .price = 0.0};
					reduceOrderQuantity(hash, &newOrder);
				}	
				else if (transaction == 'C') {
					scanf(inFile, " %d %s %d", &id, symbol, &quantity);
					newOrder = {.id = id, .symbol = symbol, .side ='a',
						.quantity = quantity, .price = 0.0};
					reduceOrderQuantity(hash, &newOrder);

				}	
				else if (transaction == 'R') {
					scanf(inFile, " %d %s %d %lf", &id, symbol, &quantity, &price);
					int index = hash->funcHash(id);
					newOrder = {.id = id, .symbol = symbol, .side = 'a',
						.quantity = quantity, .price = price};
					changeOrder(hash, &newOrder);
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
		if (foundOutput) {
			FILE *outFile = fopen(inputFile,"w");
			if (storagetype == LLIST) {
				if (foundAscending) {
					sort(hash->table, &swap, &idCompare);
				}
				printList(hash->table, hash->printItem, outFile);
			}
			else {
				printOrderBook(hash, outFile);
			}
		}
		else {
			//print to stdout
		}
		freeOrderBook(hash);
		return 0;
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

int idCompare(NodePtr n1, NodePtr n2) {
	return n1->data->id - n2->data->id;
}

void swap(NodePtr *head, NodePtr n1, NodePtr n2) {
	if (n1 == n2) {
		return;
	}
	NodePtr temp1 = nodeGetPrev(n1);
	evictNode(head, n1);
	insertNode(head,nodeGetPrev(n2), n1);
	if (temp1 != n2) {
		evictNode(head, n2);
		insertNode(head, temp1, n2);
	}
	return;
}

void sort(NodePtr *head,
		void (*swapPtr)    (NodePtr *, NodePtr, NodePtr),
		int  (*comparePtr) (void*, void*)) {
	int n, length, sorted;
	NodePtr currentNode, nextNode;
	sorted = 0;
	while(!sorted) {
		sorted = 1;
		currentNode = *head;
		while( currentNode != NULL) {
			nextNode = nodeGetNext(currentNode);
			if (nextNode != NULL) {
				if (comparePtr(currentNode, nextNode) > 0) {
					swapPtr(head, currentNode, nextNode);
					sorted = 0;
				}
			}
			currentNode = nodeGetNext(currentNode);
		}
	}
}
