#define MAXNUMARG 20
#define MAXARGLEN 100 
#define MAXBUFFSIZE 100
#define MAXCOMMANDSIZE 400

#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <ctype.h>
#include "myTerminal.h"
#include <sys/stat.h> 
#include <fcntl.h>

int isValidArg(char *arg);

void parseCommand(char ***argv, char *string, int *argc);

void freeArgList(char ***argv, int argc);

void execCommand(int argc, char** argv, int fdWrite);

int main(int argc, char **argv) {

	if (argc != 2) {
		perror("invalid number of arguments\n");
		return 1;
	}

	FILE *commandFile = fopen(argv[1], "r");

	if (commandFile == NULL) {
		perror("fopen failed\n");
		return 1;
	}

	char *tempString = malloc(sizeof(char)*MAXCOMMANDSIZE);

	int pipefd[2];
	pid_t cpid;

	int k = 1;
	char *parentBuf = malloc(sizeof(char)*MAXBUFFSIZE);

	char **argv1;
	int argc1;

	while((tempString = fgets(tempString, MAXCOMMANDSIZE, commandFile)) != NULL) {

		write(STDOUT_FILENO,"> ",sizeof(char)*2);
		write(STDOUT_FILENO,tempString,sizeof(char)*strlen(tempString) + 1);
		argc1 = 0;

		if (isValidArg(tempString)) { /*If it is not empty or only with spaces*/

			parseCommand(&argv1, tempString, &argc1);

			if (pipe(pipefd) == -1) {
				perror("pipe failed\n");
				exit(EXIT_FAILURE);
			}

			cpid = fork();

			if (cpid == -1) {
				perror("fork failed\n");
				exit(EXIT_FAILURE);
			}
			
			if (cpid == 0) { /*if it is the child process*/
				
				close(pipefd[0]);
				execCommand(argc1, argv1, pipefd[1]);
				return 0;
			}
			else {
				close(pipefd[1]);
				read(pipefd[0],parentBuf,MAXBUFFSIZE - 1);
				if (isalpha(parentBuf[0])) {
					write(STDOUT_FILENO, parentBuf, strlen(parentBuf));
					write(STDOUT_FILENO, "\n", 1);
				}
			}

			freeArgList(&argv1, argc1);
		}

		memset(tempString, '\0', MAXCOMMANDSIZE);

		write(STDOUT_FILENO, "\n", 1);
		k++;

	}
	
	write(STDOUT_FILENO, "> ", 2);

	free(tempString);
	tempString = NULL;
	free(parentBuf);
	parentBuf = NULL;

	return 1;
}

int isValidArg(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if (isalnum(str[i]) || str[i] == '>'
				|| str[i] == '|' || str[i] == '&') {
			return 1;
		}
	}
	return 0;
}

void parseCommand(char ***argv, char *string, int *argc) { /*special cases: "\"", " " */

	*argv = malloc(sizeof(char *)*MAXNUMARG);
	int i,k;
	char c;
	i = 0;

	while(1) {
		k = 0;
		char *substr = malloc(sizeof(char)*MAXARGLEN);
		while((c = string[i]) != '\0') {
			if (c == ' ') {
				i++;
				break;
			}
			else if (c == '\"') {
				substr[k] = c;
				i++;
				k++;
				while((c = string[i]) != '\"') {
					substr[k] = c;
					k++;
					i++;
				}
				substr[k] = c;
				i++;
				break;
			}
			else  {
				substr[k] = c;
				k++;
				i++;
			}
		}
		if (isValidArg(substr)) {
			substr[k+1] = '\0';
			(*argv)[*argc] = substr;
			*argc = (*argc) + 1;
		}
		if (c == '\0') {
			break;
		}
	} 
}

void freeArgList(char ***argv, int argc) {
	int i;
	for(i = 0 ; i < argc; i++) {
		free((*argv)[i]);
		(*argv)[i] = NULL;
	}
	free(*argv);
	*argv = NULL;
}
