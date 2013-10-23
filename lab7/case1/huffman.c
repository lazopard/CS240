#define MAXSTRINGSIZE 140;
#define BUFFERSIZE 1000
#define INPUTFILE "-ib"
#define OUTPUTFILE "-o"
#define CODEFILE "--code"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned char charToCode(char c, FILE *code);

int getCharArraySize(FILE *code);

void clearStr(char *string);

int main(int argc, char **argv) {
	int i, decode, c, byteCount, charCode;
	const char *inputFile, *outputFile, *codeFile;
	char *buffer = (char *) malloc(sizeof(char)*BUFFERSIZE);

	if (argc < 7) {
		printf("Missing neccesary arguments\n");
		return 1;
	}

	for(i = 1; i < argc; i++) { //deal with command-line arguments
		if (!strncmp(argv[i], INPUTFILE,2)) {
			if (!strcmp(argv[i], INPUTFILE))  //input file is binary
				decode = 1;
			else 
				decode = 0;
			inputFile = argv[i+1];
			i++;
		}

		else if (!strcmp(argv[i], OUTPUTFILE)) {
			outputFile = argv[i+1];	
			i++;
		}

		else if (!strcmp(argv[i], CODEFILE)) {
			codeFile = argv[i+1];
			i++;
		}
		else { //unknown command-line argument
			printf("Invalid argument");
			return 1;
		}
	}


	FILE *input = fopen(inputFile, "r");
	if (input == NULL) {
		printf("File %s does not exist.\n", inputFile);
		return 1;
	}
	FILE *output = fopen(outputFile, "w");
	if (output == NULL) {
		printf("File %s does not exist.\n", outputFile);
		return 1;
	}
	FILE *code = fopen(outputFile, "r");
	if (code == NULL) {
		printf("File %s does not exist.\n", codeFile);
		return 1;
	}

	int charArraySize = getCharArraySize(code);
	char charArray[charArraySize];
	char codeArray[charArraySize];
	int size= MAXSTRINGSIZE;
	char *tempString = malloc(sizeof(char)*size);
	i = 0;
	while((tempString = fgets(tempString, size, code)) != NULL) {
		charArray[i] = *tempString;
		codeArray[i] = *(strrchr(tempString, '=') + 1);
		clearStr(tempString);
	}
	free(tempString);

	//encode input
	if (!decode) {			
	}
	//decode input
	else {
	}

	free(buffer);
	fclose(input);
	fclose(output);
	fclose(code);
	return 0;
}

unsigned char charToCode(char c, FILE *code) {
	char temp;
	while((temp = fgetc(code)) != EOF) {
		if (temp == c) {
			fgetc(code);
			return fgetc(code);
		}
	}
	return 1;
}

char codeToChar(unsigned char c, FILE *code) {
	unsigned char temp;
	while((c = fgetc(code)) != EOF) {
		if (temp == c) {
			ungetc(temp,code);
			return (ungetc(temp,code));
		}
	}
	return 1;
}

int getCharArraySize(FILE *code) {
	int count = 0;
	char c;
	while((c = fgetc(code)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
	return count;
}

void clearStr(char *string) {
	int i = 0;
	for(i; i < strlen(string); i++) {
		string[i] = '\0';
	}
}
