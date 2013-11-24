#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*compare KEYWORD with a STRING and return 1 if equal, 0 if not*/

int isDelimiter(char c);

int keyCmp(const char *keyword, char *string, int currentIndex) {
	int i;
	int keyLen = strlen(keyword);
	for(i=0;i < keyLen; i++) {
		if (string[i + currentIndex] != keyword[i])
			return 0;
	}
	if (currentIndex == 0)
		return (isDelimiter(string[i + currentIndex])); /*if the word is isolated return 1*/
	else
		return (isDelimiter(string[i + currentIndex]) && isDelimiter(string[currentIndex - 1]));
}

/*check if the char C is a delimiter as defined in the handout*/

int isDelimiter(char c) {
	return (c == ' ' || c == '\t' || c == '\n');
}

/*given a STRING count number of times KEYWORD appears*/

int countWords(char *string, const char *keyword) {
	int i = 0;
	int count = 0;
	char c;
	while((c = string[i]) != EOF) {
		if (c == keyword[0]) {
			if (keyCmp(keyword, string + i, i))
				count++;
		}
	}
	return count;
}

int main() {
	const char *keyword = "dream";
	int keylen = strlen(keyword);
	FILE *input = fopen("adreamwithinadream", "r");

	fclose(input);
}
