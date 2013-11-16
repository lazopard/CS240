#define MAXWORLEN 50
#define MAXCOMMANDSIZE 400

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myTerminal.h"
#include <sys/stat.h> 
#include <fcntl.h>

int containsAlphaNum(char *);

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

	int fd = fileno(commandFile);
	char *tempString = malloc(sizeof(char)*MAXCOMMANDSIZE);
	FILE *newFile = fopen("new.txt","w+");
	int k = 1;

	while((tempString = fgets(tempString, MAXCOMMANDSIZE, commandFile)) != NULL) {

		/*fprintf(stdout,"> %s", tempString);*/
		fprintf(newFile,"> %s", tempString);
		argc = 0;

		if (containsAlphaNum(tempString)) { /*If it is not empty or only with spaces*/

			int len = strlen(tempString);
			char **argv = malloc(sizeof(char*)*len);
			int argc = 0;

			parseCommand(&argv, tempString, &argc);

			/*execCommand(argc, argv, fd);*/

			freeArgList(&argv, argc);

		}

		memset(tempString, '\0', MAXCOMMANDSIZE);
		/*fputc('\n', stdout);*/
		fputc('\n', newFile);
		k++;

	}

	fprintf(newFile,"> ");
	fclose(newFile);
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


void parseCommand(char ***argv, char *string, int *argc) { /*special cases: "\"", " " */
	int i,k;
	i = 0;
	char c;
	while(1) {
		k = 0;
		char *substr = malloc(sizeof(char)*MAXWORLEN);
		while((c = string[i]) != '\0') {
			if (c == ' ') {
				break;
			}
			else if (c == '\"') {
				substr[k] = c;
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
			}
			i++;
		}
		if (c == '\0') {
			break;
		}
		if (containsAlphaNum(substr)) {
			(*argv)[*argc] = substr;
			*argc++;
		}
	}
}

void freeArgList(char ***argv, int argc) {
	int i;
	for(i = 0 ; i < argc; i++) {
		printf("%s\n",(*argv)[i]);
		free((*argv)[i]);
		(*argv)[i] = NULL;
	}
	free(*argv);
	*argv = NULL;
}
