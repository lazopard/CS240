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

void printListStdOut(NodePtr *head);

void swap(NodePtr *head, NodePtr n1, NodePtr n2);

void sort(NodePtr *head,
				void (*swapPtr)(NodePtr *, NodePtr, NodePtr),
				int  (*comparePtr) (void*, void*));

int myHash(int id);

void printOrder(struct order *, FILE *);

int idCompare(NodePtr n1, NodePtr n2);

int main(int argc, char **argv) {
		int i, foundInput, foundOutput, foundHashtable, foundDefault, storageType, foundAscending;
		foundInput=foundOutput=foundHashtable=foundDefault=storageType = 0;
		struct hashStorage *hash;
		int (*myHashPtr)(int);
		myHashPtr = &myHash;
		void (*printOrderPtr)(struct order *, FILE *);
		printOrderPtr = &printOrderData;
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
				hash = createHash(HASHTABLESIZE, NULL, printOrderPtr);
		}
		else {
				if (foundHashtable) {
						hash = createHash(HASHTABLESIZE, myHashPtr, printOrderPtr);
				}
				else {
						hash = createHash(HASHTABLESIZE, &default_hash, printOrderPtr);
				}
		}

		char side, transaction;
		char symbol[MAX_SYMBOL_LENGTH];
		int id, quantity, index;
		double price;
		struct order newOrder;

		if (foundInput) { 
				FILE *inFile = fopen(inputFile,"r");
				while((transaction = fgetc(inFile)) != EOF) {
						if (transaction == 'A') {
								fscanf(inFile, " %d %c %s %d %lf", &id, &side, symbol, &quantity, &price);
								newOrder.id = id;
								newOrder.side = side;
								strcpy(newOrder.symbol, symbol);
								newOrder.quantity = quantity; 
								newOrder.price = price;
								addOrder(hash, &newOrder);
						}
						else if (transaction == 'X') {
								fscanf(inFile, " %d %s", &id, symbol);
								if (foundHashtable || foundDefault) {
										int index = hash->funcHash(id);
										cancelOrder(hash, (*(hash->table + index))->data);
								}
								else {
										cancelOrder(hash, (getOrderNode((*(hash->table)), id))->data);
								}
						}
						else if (transaction == 'T') {
								fscanf(inFile, " %d %s %d", &id, symbol, &quantity);
								newOrder.id = id;
								strcpy(newOrder.symbol, symbol);
								newOrder.side = 'a';
								newOrder.quantity = quantity ;
								newOrder.price = 0.0;
								reduceOrderQty(hash, &newOrder);
						}	
						else if (transaction == 'C') {
								fscanf(inFile, " %d %s %d", &id, symbol, &quantity);
								newOrder.id = id;
								strcpy(newOrder.symbol, symbol);
								newOrder.side = 'a';
								newOrder.quantity = quantity;
								newOrder.price = 0.0;
								reduceOrderQty(hash, &newOrder);
						}	
						else if (transaction == 'R') {
								fscanf(inFile, " %d %s %d %lf", &id, symbol, &quantity, &price);
								index = hash->funcHash(id);
								newOrder.id = id;
								strcpy(newOrder.symbol, symbol);
								newOrder.side = 'a';
								newOrder.quantity = quantity;
								newOrder.price = price;
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

		else { //use stdin
				while((transaction = getchar()) != EOF) {
						if (transaction == 'A') {
								scanf(inFile, " %d %c %s %d %lf", &id, &side, symbol, &quantity, &price);
								newOrder.id = id;
								newOrder.side = side;
								strcpy(newOrder.symbol, symbol);
								newOrder.quantity = quantity;
								newOrder.price = price;
								addOrder(hash, &newOrder);

						}
						else if (transaction == 'X') {
								scanf(inFile, " %d %s", &id, symbol); 
								if (foundHashtable || foundDefault) {
										index = hash->funcHash(id);
										cancelOrder(hash, (*(hash->table + index))->data);
								}
								else {
										cancelOrder(hash, (getOrderNode(*(hash->table), id))->data);
								}		
						}
						else if (transaction == 'T') {
								scanf(inFile, " %d %s %d", &id, symbol, &quantity);
								newOrder.id = id;
								strcpy(newOrder.symbol, symbol);
								newOrder.side ='a';
								newOrder.quantity = quantity;
								newOrder.price = 0.0;
								reduceOrderQty(hash, &newOrder);
						}	
						else if (transaction == 'C') {
								scanf(inFile, " %d %s %d", &id, symbol, &quantity);
								newOrder.id = id;
								strcpy(newOrder.symbol, symbol);
								newOrder.side ='a';
								newOrder.quantity = quantity;
								newOrder.price = 0.0;
								reduceOrderQty(hash, &newOrder);

						}	
						else if (transaction == 'R') {
								scanf(inFile, " %d %s %d %lf", &id, symbol, &quantity, &price);
								int index = hash->funcHash(id);
								newOrder.id = id;
								strcpy(newOrder.symbol, symbol);
								newOrder.side = 'a';
								newOrder.quantity = quantity;
								newOrder.price = price;
								changeOrder(hash, &newOrder);
						}
						else if (transaction == '\n') {
								continue;
						}
						else {
								printf("Invalid transaction\n");
						}
						clearStr(symbol);
						clearstr(newOrder.symbol);
				}
		}

		if (foundOutput) {
				FILE *outFile = fopen(inputFile,"w");
				if (storageType == LLIST) {
						if (foundAscending) {
								sort(hash->table, &swap, &idCompare);
						}
						printList(*(hash->table), hash->printItem, outFile);
				}
				else {
						printOrderBook(hash, outFile);
				}
		}
		else {
				if (storageType == LLIST) {
						if (foundAscending) {
								sort(hash->table, &swap, &idCompare);
						}
						printListStdout(*(hash->table));
				}
				else {
						for(i = 0; i < hash->size; i++) {
								if ((hash->table + i) == NULL) {
										continue;
								}
								else {
										printListStdOut(hash->table + i);
								}
						}
				}
		}
		freeOrderBook(&hash);
		return 0;
}

printListStdout(NodePtr *head) {
		NodePtr currentNode = *head;
		for(;currentNode != NULL; currentNode = currentNode->next) {
				printf(out, "%d %c %s %d %f\n", currentNode->data->id, currentNode->data->side,
								currentNode->data->symbol, currentNode->quantity,
								currentNode->price);
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
		NodePtr temp1 = getPrevNode(n1);
		evictNode(head, n1);
		insertNode(head,getPrevNode(n2), n1);
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
						nextNode = getNextNode(currentNode);
						if (nextNode != NULL) {
								if (comparePtr(currentNode, nextNode) > 0) {
										swapPtr(head, currentNode, nextNode);
										sorted = 0;
								}
						}
						currentNode = getNextNode(currentNode);
				}
		}
}

void printOrderData(OrderPtr orderData, FILE *out) {
		fprintf(out, "%d %c %s %d %lf", orderData->id, orderData->side, orderData->symbol, orderData->quantity, orderData->price);
		return;
}
