#define MAXLENGTH 140
#include <stdio.h>


int is_alpha_numeric(char c) {
	return c >= 'A' && c <='Z' || c >= 'a' && c<='z' || c >= '0' && c <='9';
}

int main() {
	char message[MAXLENGTH];
	int i,count,j = 0;
	char c;
	while (( (c = getchar()) != EOF) && count < MAXLENGTH) {
		message[count++] = c;
	}
	for(i=0; i < count;i++) {
		if (message[i] == '@' && is_alpha_numeric(message[i+1])) 
			j = i - 1 ;
			while (is_alpha_numeric(message[j]))
					message[j--] = 'X';
	}
	for(i = 0;i < MAXLENGTH;i++)
		putchar(message[i]);
	putchar('\n');
}
