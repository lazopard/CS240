#define ONLYNEWLINE -13
#define SIZE 140
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

int readMsg(char *buf);

void resetString(char *buf);

int wordLen(char* str, int strLen, int start);

int getNextWordIndex(char *string, int len, int prevEnd);

int main() {
		int listLength, line, strlength, i, sublength;
		char *msg = (char *) malloc(sizeof(char)*(SIZE + 1));
		char *substr;
		i = sublength = line = listLength = strlength = 0;
		struct lnode **head;
		while( (strlength = readMsg(msg)) != EOF) {
				if (strlength != ONLYNEWLINE) {
						while ( (i = getNextWordIndex(msg, strlength, i)) != -1) {
								sublength = wordLen(msg, strlength,i);
								if (sublength == 0)
									break;
								substr = (char *)  malloc(sizeof(char)*sublength);
								substr = strncpy(substr, msg + i, sublength);
								printf("sublength is %d\n",sublength);
								resetString(substr);
								i += sublength;
						}
				}
				i = 0;
				resetString(msg);
				line++;
		}
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
		if (c == '\n' && len > 0) {
				*(buf + len) = c;
				return len + 1;
		}
		if (c == '\n' && len == 0) {
			return ONLYNEWLINE;	
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

int isWord(char *str, int sublength) {
	
}
