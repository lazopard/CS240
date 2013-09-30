#include <stdio.h>
#include "words.h"

int isAlphaNumeric(char c) {
	return (c >= 'a' && c<= 'z' || c>= 'A' && c <= 'Z' || c >= '0' && c<= '9');
}

int main(int argc, char** argv) {
	char msg[SIZE];
	int templen,totlen;
	totlen=0;
	while( (templen = readMsg(msg) ) != EOF) { 
		totlen += templen;	
		lowerStringCase(msg,templen);
		int i = 0;
		int j = 1;
		for(j;j<argc;j++) {
			matchAndEraseWord(msg,i,wordLen(msg,templen,i),argv[j]);
		}
		while(i != -1) {
			while(msg[i] != ' ' && isAlphaNumeric(msg[i]))
				i++;
			i=getNextWordIndex(msg,templen,i);
			for(j=1;j<argc;j++) 
				matchAndEraseWord(msg,i,wordLen(msg,templen,i),argv[j]);
		}
		for(i=0;i<templen;i++)
			putchar(msg[i]);
			msg[i] = '\0';
	}
}
