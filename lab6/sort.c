/**
 * CS240 Lab 6 sort.c
 * Basic comparators, node swapper, and bubble sort
 *
 * @author	YOUR_NAME_HERE
 * @date	DATE_FINISHED_HERE
 */

#include <stdio.h>
#include <string.h>
#include "list.h"
#include "sort.h"

int wordCmp (struct lnode* n1, struct lnode* n2) {
	return (strcmp(nodeGetWord(n1),nodeGetWord(n2)));
}

int lineCmp (struct lnode* n1, struct lnode* n2) {
	int difference = nodeGetLine(n1) - nodeGetLine(n2);
	if (difference == 0) 
		return wordCmp(n1,n2);
	else
		return difference;
}

int countCmp (struct lnode* n1, struct lnode* n2) {
	int difference = nodeGetCount(n1) - nodeGetCount(n2);
	if (difference == 0) 
		return wordCmp(n1,n2);
	else
		return difference;
}

void swap (struct lnode** head, struct lnode* n1, struct lnode* n2) {
	if (n1 == n2) {
		return;
	}
	struct lnode *temp1 = nodeGetPrev(n1);
	evictNode(head, n1);
	insertNode(head,nodeGetPrev(n2), n1);
	if (temp1 != n2) {
		evictNode(head, n2);
		insertNode(head, temp1, n2);
	}
	return;
}

/* Hint: don't forget to use function pointers to reuse code wisely */

int llistGetLength(struct lnode **head);

void sort (struct lnode** head,
			void (*swapPtr)    (struct lnode**, struct lnode*, struct lnode*),
			int  (*comparePtr) (void*, void*)) {
	int length, newn, i, j;
	struct lnode *nextNode, *currentNode;
	currentNode = (*head);
	nextNode = nodeGetNext(currentNode);
	length = llistGetLength(head);
	for(i = 0; i < length; i++) {
		while(nextNode != NULL) {
			if(comparePtr(currentNode, nextNode) < 0) {
				swapPtr(head, currentNode, nextNode);
			}
			currentNode = nodeGetNext(currentNode);
			nextNode = nodeGetNext(currentNode);
		}
	}
}

void sortByWord (struct lnode** head){
	void (*swapPtr)(struct lnode **, struct lnode *, struct lnode *);
	swapPtr = &swap;
	int (*cmpPtr)(struct lnode*, struct lnode*);
	cmpPtr = &wordCmp;
	sort(head, swapPtr, cmpPtr); 
}

void sortByLine (struct lnode** head){
	void (*swapPtr)(struct lnode **, struct lnode *, struct lnode *);
	swapPtr = &swap;
	int (*cmpPtr)(struct lnode*, struct lnode*);
	cmpPtr = &lineCmp;
	sort(head, *swapPtr, cmpPtr);
}

void sortByCount (struct lnode** head){
	void (*swapPtr)(struct lnode **, struct lnode *, struct lnode *);
	swapPtr = &swap;
	int (*cmpPtr)(struct lnode*, struct lnode*);
	cmpPtr = &countCmp;
	sort(head, *swapPtr, cmpPtr);
}

int llistGetLength(struct lnode **head) {
	int length = 0;
	struct lnode *temp = *head;
	while(temp != NULL) {
		temp = nodeGetNext(temp);	
		length++;
	}
	return length;
}
