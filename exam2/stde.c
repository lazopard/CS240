#include <stdio.h>

int main() {
	char buff[16];
	fprintf(stderr,"What is your name?\n");
	scanf("%s", buff);
	fprintf(stderr, "Your name is %s\n",buff);

	return 0;
}

