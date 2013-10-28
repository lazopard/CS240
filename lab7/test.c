#include <stdio.h>

/*
unsigned char convertToBinary(char *binaryString) {
	unsigned char binary = 00000000;
	char tempBit = 0;
	int i, j, twoPow
	i = strlen(binaryString) - 1;
	while(i > 0) {
		if (*(binaryString + i) == 1) {
			for(j = 0;j < i;j++) {
				twoPow *= 2;
			}
			binary | twoPow;
		}
		i--;
	}
	return binary; 
}
*/

int main() {
	int i =5;
	int j = 0;
	int twoPow = 1;
	for(j; j < 5; j++) {
		twoPow *= 2;
	}
	printf("twoPow is %d\n", twoPow);
}
