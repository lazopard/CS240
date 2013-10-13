#include <stdio.h>
#include "utils.h"
#include "words.h"

int isAlphaNumeric(char c) {
	if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9')
		return 1;
	return 0;
}

void lowerStringCase(char* str, int len) {
	int i;
	for(i = 0; i < len; i++) {
		*(str + i) = lowerCaseOf(*(str + i));
	}
}

int isSpace(char c) {
	if (c == '\n' || c == ' ' || c == '\t' || c == '\v' || c == '\f') {
		return 1;
	}
	return 0;
}

int wordLen(char* str, int strLen, int start){
	int i;
	for(i=0; i < strLen; i++) {
		if (i == start)  {
			int j = 0;
			while(!isSpace(*(str + i))) {
				j++;
				i++;
			}
			return j;
		}
	}
	return 0;
}

void matchAndEraseWord(char *str, int start, int wLen, char* keyword) {
     int i, count;
     char *p = &(*(str + start));
     if (strNCmp(p,keyword,wLen)) {
     	for(i=0; i < wLen; i++)
     		*(p + i) = ' ';
     }
     return;
}

int getNextWordIndex(char *string, int len, int prevEnd) {
	if (prevEnd > len || *string == '\0') {
		return -1;
	}
	if (*string == '\0' && prevEnd == 0) {
		return -1;
	}
	int i = 0;
	while(!(isAlphaNumeric(*(string + i + prevEnd)))) {
		i++;
	}
	return (i + prevEnd);
}

/*
 It reads characters from stdin until either the buffer is full (SIZE chars are read), or it encounters EOF or '\n'.

 Return the number of chars written to the buffer (at most SIZE) unless the first char it reads is EOF and in this situation, it returns EOF.

 Note that it doesn't treat the chars as strings, so no '\0' terminator will be appended at the end.
 */

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
	else if (c == '\n') {
		*(buf + len) = c;
		return len + 1;
	}
	else {
		return len;
	}
}
