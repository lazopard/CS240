#define SIZE 140
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int isAlpha(char c);

void printWordCount(struct lnode **head);

struct lnode *chooseBestDeletion(struct lnode *a, struct lnode *b);

void resetBuffer(char *buf);

/*
*read tweet from stdin until there are no more, store in char **msg
*process per tweet
*initialize the head of the list, have a counter for the length
*go through every word in msg, add it to the list
*if size of list is 100
*choose which word to take out: // done
*print the list with appropiate format //done
*free all memory
*/

int main() {
	int listLength, line, strlength;
	char *msg = (char *) malloc(sizeof(char)*(SIZE + 1));
	char *substr;
	while( (strlength = readMsg(msg)) != EOF) {

		resetBuffer(msg);
		line++;
	}
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

int readMsg(char* buf) {
		int len;
		int c;
		len=0;
		while((c = getchar()) != EOF && c != '\n' && len < SIZE) {
				*(buf + len) = c;
				len++;
		}
		if (len == 0 && c == EOF) {
				return EOF;
		}
		if (c == '\n') {
				*(buf + len) = c;
				return len + 1;
		}
		return len;
}

int wordLen(char* str, int strLen, int start){ 
		int i; 
		for(i=0; i < strLen; i++) { 
				if (i == start)  { 
						int j = 0; 
						while(isAlpha(*(str + i))) { 
								j++; 
								i++; 
						} 
						return j; 
				} 
		} 
		return 0; 
}

int getNextWordIndex(char *string, int len, int prevEnd) {
		if (prevEnd > len || *string == '\0') {
				return -1;
		}
		if (*string == '\0' && prevEnd == 0) {
				return -1;
		}
		int i = 0;
		while(isAlphaNumeric(*(string + i + prevEnd))) {
				i++;
		}
		return (i + prevEnd);
}

void resetBuffer(char *buf) {
		int i = 0;
		while(*(buf + i) != '\0') {
				*(buf + i) = '\0';
				i++;
		}
}
