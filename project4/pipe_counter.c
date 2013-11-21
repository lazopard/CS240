#define INPUT "-i"
#define OUTPUT "-o"
#define BUFFERSIZE "-b"
#define KEYWORD "-k"

#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int wordCount = 0;

int keyCmp(const char *keyword, char *string, int keyLen, int currentIndex);

int main(int argc, char **argv) {

	/*Args processing*/

	if (argc != 9) {
		printf("invalid number of args\n");
		return 1;
	}

	char *input, *output;
	const char *keyword;
	int bufferSize;

	int foundInput, foundOutput, foundKeyword, foundBufferSize;
	foundInput=foundOutput=foundKeyword=foundBufferSize=0;

	int i;

	for(i=1;i < argc;i++) {

		if (!strcmp(argv[i], INPUT)) {
			input = argv[i+1];
			i++;
		}
		else if (!strcmp(argv[i], OUTPUT)) {
			output = argv[i+1];
			i++;
		}
		else if (!strcmp(argv[i], KEYWORD)) {
			keyword = argv[i+1];
			int key_len = strlen(keyword);
			i++;
		}
		else if (!strcmp(argv[i], BUFFERSIZE)) {
			bufferSize = atoi(argv[i+1]);
			i++;
		}
		else {
			printf("Invalid argument...\n");
			return 1;
		}
	}
	/*Args processing end*/

	/*Open files*/

	FILE *inputFile = fopen(input, "r");

	if (inputFile == NULL) {
		perror("error opening input file...\n");
		return 1;
	}

	FILE *outputFile = fopen(output, "w");

	if (outputFile == NULL) {
		perror("error opening output file...\n");
		return 1;
	}

	/* get size of input file */

	fseek(inputFile, 0, SEEK_END);
	int size = ftell(inputFile);
	rewind(inputFile);

	/* get number of forks to execute*/

	int numForks = size/bufferSize;

	/*get number of bytes for main*/

	if ((size % bufferSize) == 0) {
		return 1;
	}

	int mainBytes = size % bufferSize;

	/*Initialize pipes*/

	int pipe_fds[2];
	int read_fd;
	int write_fd;
	pipe(pipe_fds);
	read_fd = pipe_fds[0];
	write_fd = pipe_fds[1];

	/*word counting starts */

	pid_t cpid;

	for(i = 0; i < numForks;i++) {
		cpid = fork();
		if (cpid < 0) {
			perror("fork failed...\n");
			return 1;
		}
		else if (cpid == 0) { /*child process*/
		}
		else { /*parent process*/
		}
	}

	/*free memory and close files*/

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}

int isDelimiter(char c);

/*compare string with keyword*/

int keyCmp(const char *keyword, char *string, int keyLen, int currentIndex) {
	int i;
	for(i=0;i < keyLen; i++) {
		if (string[i + currentIndex] != keyword[i])
			return 0;
	}
	if (currentIndex == 0)
		return (isDelimiter(keyword[i + currentIndex])); /*if the word is isolated return 1*/
	else
		return (isDelimiter(string[i + currentIndex]) && isDelimiter(string[currentIndex - 1]));
}

/*check if the char C is a delimiter as defined in the handout*/

int isDelimiter(char c) {
	return (c == ' ' || c == '\t' || c == '\n');
}

