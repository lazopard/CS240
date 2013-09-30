#define MAXLENGTH 140
#include <stdio.h>

int main() {
	char message[MAXLENGTH+1];
	int i,count = 0;
	char c;
	while ((c = getchar()) != EOF && count < MAXLENGTH) {
		message[count++] = c;
	}
	for(i =0; i < MAXLENGTH;i++)
		putchar(message[i]);
	putchar('\n');
}
