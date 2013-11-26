#define INPUT "-i"
#define OUTPUT "-o"
#define BUFFERSIZE "-b"
#define KEYWORD "-k"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int countWords(char *string, const char *keyword);

int main(int argc, char **argv) {
	
	char *input_file_name = "pipe_counter.c";
	char *file_contents;
	long input_file_size;
	FILE *input_file = fopen(input_file_name, "r");
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	rewind(input_file);
	file_contents = malloc(input_file_size * (sizeof(char)));
	fread(file_contents, sizeof(char), input_file_size, input_file);
	int wordCount = countWords(file_contents,"int");
	printf("count is %d\n", wordCount);
	fclose(input_file);

	return 0;
}

/*check if the char C is a delimiter as defined in the handout*/

int isDelimiter(char c) {
	return (c == ' ' || c == '\t' || c == '\n');
}

/*given a STRING count number of times KEYWORD appears*/

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
	while((c = string[i]) != '\0') {
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
		else {
			i++;
		}
	}
	return count;
}
