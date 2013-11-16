#define MAXARGLEN 100

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
	int i,k;
	i = 0;
	char c;
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
			(*argv)[*argc] = substr;
			*argc = (*argc) + 1;
		}
		if (c == '\0') {
			break;
		}

	}
}

int main(void) {
	char **argv = malloc(sizeof(char *)*100);
	char *string = "gcc \"hello world\" >  txt.txt\0";
	int argc = 0;
	parseCommand(&argv, string, &argc);
	int i;
	for(i = 0; i < argc; i++) {
		printf("%s\n",argv[i]);
	}
	return 0;
}

