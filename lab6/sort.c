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
	return strcmp(nodeGetWord(n1),nodeGetWord(n2));
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

int llistGetLength(struct lnode **head);


void sort (struct lnode** head,
			void (*swapPtr)    (struct lnode**, struct lnode*, struct lnode*),
			int  (*comparePtr) (void*, void*)) {
<<<<<<< HEAD
	int n, length, sorted;
	struct lnode *currentNode, *nextNode;
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
=======
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
>>>>>>> 56f9c868ddf5f6923543a49aa901dcb5dd610197
		}
	}
}

void sortByWord (struct lnode** head){
	sort(head, &swap, &wordCmp); 
}

void sortByLine (struct lnode** head){
	sort(head, &swap, &lineCmp); 
}

void sortByCount (struct lnode** head){
	sort(head, &swap, &countCmp);
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

int main() {
		struct lnode **headptr;
		struct lnode *inhead = newNode("z node1",6,10);
		struct lnode *node = newNode("b node2",15,50);
		struct lnode *node2 = newNode("a node3",38,90);
		struct lnode *node3 = newNode("m node4",52,1);
		struct lnode *node4 = newNode("l node5",10,53);
		struct lnode *node5 = newNode("k node6",30,99);
		struct lnode *node6 = newNode("y node7",9,15);
		struct lnode *node7 = newNode("p node8",4,57);
		struct lnode *node8 = newNode("j node9",1,92);
		struct lnode *node9 = newNode("y node10",7,1);
		headptr = &inhead;
		insertNode(headptr, inhead, node);
		insertNode(headptr, node, node2);
		insertNode(headptr, node2, node3);
		insertNode(headptr, node3, node4);
		insertNode(headptr, node4, node5);
		insertNode(headptr, node5, node6);
		insertNode(headptr, node6, node7);
		insertNode(headptr, node7, node8);
		insertNode(headptr, node8, node9);

		printf("\nsort by line:\n\n");
		sortByLine(headptr);
		printList(inhead);
		printf("\nsort by count:\n\n");
		sortByCount(headptr);
		printList(inhead);
		printf("\nsort by word:\n\n");		
		sortByWord(headptr);
		printList(inhead);
}
