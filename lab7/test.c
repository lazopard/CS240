#include <stdio.h>
#include "string.h"

int twoToThePowerOf(int i) {
				int j, twoPow;
				twoPow = 1;
				for(j = 0;j < i;j++) {
								twoPow *= 2;
				}
				return twoPow;
}

unsigned char convertToBinary(char *binaryString) {
				unsigned char binary = 00000000;
				char tempBit = 0;
				int i, j, k, twoPow;
				i = strlen(binaryString) - 1;
				j = 0, twoPow = 1;
				k = 0;
				while(i >= 0) {
								if (*(binaryString + k) == '1') {
												twoPow = twoToThePowerOf(i);
												binary = binary | twoPow;
												printf("binary is %d\n", binary);
								}
								twoPow = 1;
								i--;
								k++;
				}
				return binary;
}
