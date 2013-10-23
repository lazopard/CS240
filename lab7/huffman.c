#define BUFFERSIZE 10000
#define INPUTFILE "-ib"
#define OUTPUTFILE "-o"
#define CODEFILE "--code"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned char charToCode(char c, FILE *code);

void initBuffer(char **buffer);

int main(int argc, char **argv) {
	int i, decode, c, byteCount, charCode;
	const char *inputFile, *outputFile, *codeFile;
  char *buffer = (char *) malloc(sizeof(char)*BUFFERSIZE);
  initBuffer(buffer);
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

	//encode input
	if (!decode) {			
				for(i = 0;(((*(buffer + i)) = fgetc(input)) != EOF); i++);
	}
	//decode input
	else {
				for(i = 0;(((*(buffer + i)) = fgetc(input)) != EOF); i++);
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

void initBuffer(char **buffer) {
				int i = 0;
				for(i;i < BUFFERSIZE; i++) {
								*(buffer[i]) = '\0';
				}
				return;
}
