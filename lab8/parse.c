#define MAXARGLEN 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
		char *substr = malloc(sizeof(char)*MAXARGLEN);
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

int main(void) {
	char **argv = malloc(sizeof(char *)*1000);
	char *string = "gcc \"hello world\"\0";
	int argc = 0;
	parseCommand(&argv, string, &argc);
	printf("there are %d arguments\n",argc);
	int i;
	for(i = 0; i < argc; i++) {
		printf("%s\n",argv[i]);
	}
	return 0;
}

