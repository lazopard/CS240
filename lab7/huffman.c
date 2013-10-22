#define INPUTFILE "-ib"
#define OUTPUTFILE "-o"
#define CODEFILE "--code"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	int i, decode;
	int finput, foutput;
	const char inputFile, outputFile, codeFile;
	for(i =0; i < argc; i++) { //deal with command-line arguments
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

	finput = open(inputFile, O_RDONLY);
	foutput = open(outputFile, O_WRONLY);

	if (!decode) {
		
	}

	else { //decode

	}
	close(finput);
	close(foutput);
}
