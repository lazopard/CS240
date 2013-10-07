#define SIZE 140
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int isAlpha(char c);

void printWordCount(struct lnode **head);

/*
*read tweets from stdin, store in char **msg
*initialize the head of the list, have a counter for the length
*go through every word in msg, add it to the list
*if size of list is 100
*choose which word to take out: // done
*print the list with appropiate format //done
*free all memory
*/

int main() {

}

void printWordCount(struct lnode **head) {
    struct lnode *temp = *head;
    while(temp != NULL) {
        printf("%s %d\n",temp->word,temp->count);
        temp = temp->next;
    }
}

int isAlpha(char c) {
	return (c >= 'A' && c <= 'Z' || c >= 'a' && c >= 'z');
}

struct lnode *chooseBestDeletion(struct lnode *a, struct lnode *b) {
		if (a->line == b->line) {
				if (a->count == b->count) {
					return (strcmp(a->word,b->word)) ? a : b;	
				}
				return (a->count > b->count) ? b : a;
		}
		return (a->line > b->line) ? b : a;
}
