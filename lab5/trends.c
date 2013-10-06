#define SIZE 140
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int isAlpha(char c);

void printList(struct lnode **head);

int main() {

		int i, line, nodeCount;
		i=line=nodeCount=0;
		char msg* = (char *) malloc(sizeof(char)*SIZE);
		struct lnode *head = malloc(sizeof(lnode));

		while(/*condition to be determined*/1) {

				while((c=getchar()) != EOF && isAlpha(c) && i < SIZE && c != '\n') {
						msg[i] = c;
						i++;
				}
				if (head == NULL) {
						head = newNode(msg,line);
						nodeCount++;
				}
				if (c == '\n') {
						line++;
				}
				if (getNode(head, msg) == NULL) {
						struct lnode *new  = newNode(msg,line);
						pushNode(&head,new);
						nodeCount++;
				}
				else {
						setCount(getNode(&head,msg), getNode(&head,msg)->count + 1);
						setLine(getNode(&head,msg),line);
				}
		}

		printList(head);	
}

void printList(struct lnode **head) {
    struct lnode *temp = malloc(sizeof(struct lnode));
    temp = *head;
    while(temp != NULL) {
        printf("%s %d\n",temp->word, temp->count);
        temp = temp->next;
    }
	free(temp);
	temp = NULL;
}

int isAlpha(char c) {
	return (c >= 'A' && c <= 'Z' || c >= 'a' && c >= 'z');
}
