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

	/*Args processing*/

	if (argc != 9) {
		printf("invalid number of args\n");
		return 1;
	}

	char *input, *output;
	const char *keyword;
	int bufferSize;

	/*int foundInput, foundOutput, foundKeyword, foundBufferSize;
	foundInput=foundOutput=foundKeyword=foundBufferSize=0;*/

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
			i++;
		}
		else if (!strcmp(argv[i], BUFFERSIZE)) {
			bufferSize = atoi(argv[i+1]);
			if (bufferSize <= 0) {
				printf("Invalid buffer size\n");
				return 1;
			}
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

	int mainCountIsComplete = 0;

	if ((size % bufferSize) == 0) {
		mainCountIsComplete = 1;
	}

	int mainBytes = size % bufferSize;


	/*Initialize variables for counting */

	int pipe_fds[2];

	pid_t cpid;

	int childCount = 0;
	int cCount = 0; /*buffer from where count from child processes are read*/
	int totalCount = 0;

	int fileSizePerChild = (size-mainBytes)/numForks;

	/*Counting starts */

	for(i = 0; i < numForks;i++) {

		cCount=childCount=0;

		if (pipe(pipe_fds) == -1) {
			perror("pipe failed...\n");
			exit(EXIT_FAILURE);
		}

		cpid = fork();

		if (cpid < 0) { /*if ford failed*/
			perror("fork failed...\n");
			return 1;
		}
		else if (cpid == 0) { /*child process*/
			close(pipe_fds[0]);
			char *tempString = malloc(sizeof(char)*fileSizePerChild);
			fseek(inputFile,i*fileSizePerChild, SEEK_SET);
			fread(tempString, sizeof(char), fileSizePerChild, inputFile);
			childCount = countWords(tempString, keyword);
			write(pipe_fds[1], &childCount, sizeof(int));
			free(tempString);
			_exit(0);
		}
		else { /*parent process*/
			close(pipe_fds[1]);
			read(pipe_fds[0], &cCount, sizeof(int));
			totalCount += cCount;
			if (!mainCountIsComplete) {
				char *tempString = malloc(sizeof(char)*mainBytes);
				fseek(inputFile,fileSizePerChild*numForks, SEEK_SET);
				fread(tempString, sizeof(char), mainBytes, inputFile);
				totalCount += countWords(tempString, keyword);
				free(tempString);
				mainCountIsComplete = 1;
			}
		}
	}

	/*Counting ends*/

	fprintf(outputFile, "%d\n", totalCount);

	/*free memory and close files*/

	fclose(inputFile);
	fclose(outputFile);

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
