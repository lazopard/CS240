#define MAXARGLEN 100
#define MAXNUMARG 15

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
		else {
			free(substr);
		}
		if (c == '\0') {
			break;
		}
	}
}

void freeArgList(char ***argv, int argc) {
	printf("args:\n\n");
	int i;
	for(i = 0 ; i < argc; i++) {
		printf("%s\n",(*argv)[i]);
		free((*argv)[i]);
		(*argv)[i] = NULL;
	}
	free(*argv);
	*argv = NULL;
	printf("---------------------\n");
}


int main(void) {
	char **argv;
	char *string1= "gcc \"hello world\" >  txt.txt\0";
	char *string2 ="grep echo \"Wow!      <-This TAB is not an arg delimiter!\"";
	char *string3 = "sudo -s";
	char *string4 = "cat hello.c";
	int argc;
	argc = 0;
	parseCommand(&argv, string1, &argc);
	freeArgList(&argv, argc);
	argc = 0;
	parseCommand(&argv, string2, &argc);
	freeArgList(&argv, argc);
	argc = 0;
	parseCommand(&argv, string3, &argc);
	freeArgList(&argv, argc);
	argc = 0;
	parseCommand(&argv, string4, &argc);
	freeArgList(&argv, argc);
	return 0;
}

