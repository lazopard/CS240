#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isDelimiter(char c) {
	return (c == ' ' || c == '\t' || c == '\n');
}

int countWords(char *string, const char *keyword) {
	char c;
	int keylen = strlen(keyword);
	int i = 0;
	int matchlen, count;
	count = 0;
	if (!strncmp(string,keyword,sizeof(char)*keylen)
				&& isDelimiter(*(string + keylen))) {
		count++;
		i += keylen;
	} 
	while((c = string[i]) != EOF) {
		matchlen = 0;
		if (c == keyword[0] && isDelimiter(*(string + i - 1))
				&& isDelimiter(*(string + i + keylen))) {
			while((c = string[i + matchlen]) == keyword[matchlen])
				matchlen++;
			if (matchlen == keylen) {
				count++;
			}
			i += matchlen;
		}
		else 
			i++;
	}
	return count;
}

int main() {
	char *key = "dream";
	char *string = "dream within a dream within a dream\ndream or dreaming";
	int count = countWords(string, key);
	printf("count is %d\n", count);
	return 0;
}
