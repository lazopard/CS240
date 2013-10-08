#define SIZE 140
#define MAXLISTLEGNTH 100
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "list.h"

int readMsg(char *buf);

void resetString(char *buf);

int wordLen(char* str, int strLen, int start);

int getNextWordIndex(char *string, int len, int prevEnd);

void handleWord(struct lnode **head, int *listlength, int *line, char *word);

void printWordCount(struct lnode **head);

int main() {
		int listLength, line, strlength, i, sublength;
		char *msg = (char *) malloc(sizeof(char)*(SIZE + 1));
		char *substr;
		i = sublength = line = listLength = strlength = 0;
		struct lnode *head;
		while( (strlength = readMsg(msg)) != EOF) {
				while ( (i = getNextWordIndex(msg, strlength, i)) != -1) {
						sublength = wordLen(msg, strlength,i);
						substr = (char *)  malloc(sizeof(char)*sublength);
						substr = strncpy(substr, msg + i, sublength);
						resetString(substr);
						i += sublength;
				}
				i = 0;
				resetString(msg);
				line++;
		}
		free(msg);
		free(substr);
		printWordCount(&head);
		deleteList(&head);
}

int isAlpha(char c) {
	return (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z');
}

void resetString(char *buf) {
	int i = 0;
	while(*(buf + i) != '\0') {
		*(buf + i) = '\0';
		i++;
	}
}

int readMsg(char* buf) {
		int len;
		int c;
		len=0;
		while( (c = getchar()) != EOF && c != '\n' && len < SIZE) {
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

int getNextWordIndex(char *string, int len, int prevEnd) {
		if (prevEnd > len || *string == '\0') {
				return -1;
		}
		if (*string == '\0' && prevEnd == 0) {
				return -1;
		}
		int i = 0;
		while(!(isAlpha(*(string + i + prevEnd)))) {
				i++;
		}
		return (i + prevEnd);
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

void evictNode(struct lnode **head);

void handleWord(struct lnode **head, int *listlength, int *line, char *word) {
		if (getNode(*head,word) == NULL) {
			struct lnode *new = newNode(word,*line);
			if ((*listlength) >= MAXLISTLEGNTH) {
				evictNode(head);	
				pushNode(head,new);
			}
			else {
				pushNode(head,new);
				(*listlength)++;
			}
		}
		else {
			struct lnode *ptr = getNode(*head,word);
			nodeSetLine(ptr,*line);
			nodeSetCount(ptr,nodeGetCount(ptr) + 1);
		}
}

struct lnode *nodeToDelete(struct lnode *a, struct lnode *b) { 
		if (nodeGetLine(a) == nodeGetLine(b)) { 
				if (nodeGetCount(a) == nodeGetCount(b)) { 
						if (strcmp(nodeGetWord(a),nodeGetWord(b)) == 0) {
							return a;
						}
						return (strcmp(nodeGetWord(a),nodeGetWord(b)) > 0) ? a : b;    
				} 
				return (nodeGetCount(a) > nodeGetCount(b)) ? b : a; 
		} 
		return (nodeGetLine(a) > nodeGetLine(b)) ? b : a; 
}

void evictNode(struct lnode **head) {
	struct lnode *temp = *head;
	struct lnode *toDelete = *head;
	while(temp != NULL) {
		toDelete = nodeToDelete(temp, toDelete);
		temp = nodeGetNext(temp);
	}
	deleteNode(head,toDelete);
}

void printWordCount(struct lnode **head) {
    struct lnode *temp = *head;
    while(temp != NULL) {
        printf("%s %d\n",nodeGetWord(temp), nodeGetCount(temp));
        temp = nodeGetNext(temp);
    }
}
