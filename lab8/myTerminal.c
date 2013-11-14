#define MAXBUFFSIZE 1000
#define MAXCOMMANDSIZE 400

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myTerminal.h"
#include <sys/stat.h> 
#include <fcntl.h>

int containsAlphaNum(char *);

void execCommand(int argc, char** argv, int fdWrite);

int main(int argc, char **argv) {

	if (argc != 2) {
		perror("invalid number of arguments\n");
		return 1;
	}

	char *buffer = malloc(sizeof(char)*MAXBUFFSIZE);
	FILE *commandFile = fopen(argv[1], "r");
	if (commandFile == NULL) {
		perror("fopen failed\n");
		return 1;
	}
	int fd = fileno(commandFile);
	char *tempString = malloc(sizeof(char)*MAXCOMMANDSIZE);
	while((tempString = fgets(tempString, MAXCOMMANDSIZE, commandFile)) != NULL) {
		fprintf(stdout,"> %s", tempString);
		if (containsAlphaNum(tempString)) {

			execCommand(
		}
		memset(tempString, '\0', MAXCOMMANDSIZE);
		fputc('\n', stdout);
	}
	free(buffer);
	free(tempString);
	return 1;
}

int containsAlphaNum(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if (isalnum(str[i])) {
			return 1;
		}
	}
	return 0;
}

void execCommand(int argc, char** argv, int fdWrite) {

}
