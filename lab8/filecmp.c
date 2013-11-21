#include <stdio.h>

int main() {
	FILE *file1 = fopen("new.txt","r");
	FILE *file2 = fopen("case2_out.txt", "r");
	char c, d;
	int line = 0;
	c = fgetc(file1);
	d = fgetc(file2);
	while (c == d) {
		if (c == '\n' || d == '\n') {
			line++;
		}
		c = fgetc(file1);
		d = fgetc(file2);
	}
	printf("the characters that differ are (%c) in new.txt and (%c) in case2\n",c,d);
	printf("on line %d\n",line);
	return 0;
}
