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
			int argc = 0;
			char **argv = malloc(sizeof(char*)*len);
			char *substr = strtok(tempString, " ");

			while(substr != NULL) { /*does not deal with ""'s */
				argv[argc] = malloc(sizeof(char)*strlen(substr) + 1);
				strcpy(argv[argc], substr);
				substr = strtok(NULL, " ");
				argc++;
			}
			
			/*execCommand(argc, argv, fd);*/

			/*free argv start*/
			int i;
			printf("command %d\n",k);
			for(i = 0 ; i < argc; i++) {
				printf("%s\n",argv[i]);
				free(argv[i]);
				argv[i] = NULL;
			}

			free(argv);
			argv = NULL;
			/*free argv end*/
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
