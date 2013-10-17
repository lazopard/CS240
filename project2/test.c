#include <stdio.h>
#include <stdlib.h>

int main() {

		FILE *fp = fopen("msg.txt", "r");
		char *string = malloc(sizeof(char)*1000);
		char side;
		int id, quantity;
		double price;
		char transaction;
		fgetc(fp);
		fscanf(fp, " %d %c %s %d %lf", &id, &side, string, &quantity, &price);
		printf("%c %d %c %s %d %lf\n", 'a', id, side, string, quantity, price);
		printf("%c", fgetc(fp));
		printf("%c", fgetc(fp));
		printf("%c", fgetc(fp));
}
