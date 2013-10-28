#include <stdio.h>

int main() {
	FILE *test = fopen("dot.code", "r");
	printf("fgetc(code) is %c\n", fgetc(test));
}
