#define MAXLENGTH 140
#include <stdio.h>

int is_alphanumeric(char c) {
    return c >= 'A' && c <='Z' || c >= 'a' && c<='z' || c >= '0' && c <='9';
}

int main() {
	char message[MAXLENGTH];
	int i,count,j;
	char c;
	count = 0;
	while (( (c = getchar()) != EOF) && count < MAXLENGTH) {
		message[count] = c;
		count++;
	}
	for(i = 0; i < count;i++) {
		if (message[i] == '@' && is_alphanumeric(message[i+1])) {
			j = i - 1 ;
			while (is_alphanumeric(message[j]))
				message[j--] = 'X';
		}
	}
	printf("%s",message);
}
