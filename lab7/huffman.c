#define MAXBINARYSTRING 20
#define BUFFERSIZE 10000
#define INPUTFILE "-ib"
#define OUTPUTFILE "-o"
#define CODEFILE "--code"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct charBinary {
	unsigned char binary;
	char c;
};

void clearStr(char **str);

void printCharBinaryArray(struct charBinary[], int size);

int getSizeOfCharBinaryArray(FILE *code);

unsigned char convertToBinary(char *binaryString);

int main(int argc, char **argv) {
	int i, j, charCode, decode;
	i=charCode=j=decode=0;
	const char *inputFile, *outputFile, *codeFile;
	inputFile=outputFile=codeFile=NULL;
	char *buffer = (char *) malloc(sizeof(char)*BUFFERSIZE);
	if (argc < 7) {
		printf("not enough arguments\n");
		return 1;
	}
	for(i = 1; i < argc; i++) { //deal with command-line arguments
		if (!strncmp(argv[i], INPUTFILE,2)) {
			if (!strcmp(argv[i], INPUTFILE)) { //input is binary
				decode = 1;
			}
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

	FILE *input;
	if (!decode) {
		input = fopen(inputFile, "r");
	}
	else {
		input = fopen(inputFile, "rb");
	}
	if (input == NULL) {
		printf("File %s does not exist.\n", inputFile);
		return 1;
	}
	FILE *output;
	if (!decode) {
		output = fopen(outputFile, "wb+");
	}
	else {
		output = fopen(outputFile, "w");
	}
	if (output == NULL) {
		printf("File %s does not exist.\n", outputFile);
		return 1;
	}
	FILE *code = fopen(codeFile, "r");
	if (code == NULL) {
		printf("File %s does not exist.\n", codeFile);
		return 1;
	}

	int c, charBinaryArraySize;

	charBinaryArraySize = getSizeOfCharBinaryArray(code);

	fseek(code,0, SEEK_SET);	

	struct charBinary charBinaryArray[charBinaryArraySize];

	//populate charBinaryArray

	i = 0, c = 0;
	char *binaryString = malloc(sizeof(char)*MAXBINARYSTRING);
  unsigned char binary = 00000000;

	while((c = fgetc(code)) != EOF) {

		if (i < 1) {
			charBinaryArray[i].c = c;
			fgetc(code);
			binaryString = fgets(binaryString,MAXBINARYSTRING,code);
			binary = convertToBinary(binaryString);
			charBinaryArray[i].binary = binary;
			i++;
      clearStr(&binaryString);
		}

		else if (c == '\n') {
			c = fgetc(code);
			fgetc(code);
			binaryString = fgets(binaryString,MAXBINARYSTRING,code);
      printf("%s\n",binaryString);
			charBinaryArray[i].c = c;
			charBinaryArray[i].binary = binary;
			i++;
		}

		else {
			continue;
		}

	}
	free(binaryString);
	binaryString = NULL;

	c = 0;
	//printCharBinaryArray(charBinaryArray, charBinaryArraySize);

	//encode input
	 
	if (!decode) {
		while((c = fgetc(input)) != EOF) {
			for(i = 0; i < charBinaryArraySize; i++) {
				if (c == charBinaryArray[i].c) {
					buffer[j] = charBinaryArray[i].binary;
					j++;
				}	
			}
		}
		//deal with padding 0s
		fwrite(buffer, sizeof(char), j, output);
	}

	//decode input

	/* facts about decoding:
	 * ends with a period
	 * after this last period, there are only padding 0s
	 * So if we find a period either part or all of which lies in the last byte,
	 and after this period all the remaining bits are 0, then this period has to be
	 the last period
	 * You can assume that the period will not have code consisting f only 0s, or code that is 
	 longer than a byte
	 */

	else {
		//using the | operator might work			
		//| every byte with 0000000 until it doesn't change
		//then those have to be the padding 0s
	}

	free(buffer);
	fclose(input);
	fclose(output);
	fclose(code);
	buffer = NULL;
	return 0;
}


void printCharBinaryArray(struct charBinary charBinaryArray[], int size) {
	int i;
	for(i = 0; i < size; i++) {
		printf("charBinaryArray[i].c is (%c), charBinaryArray[i].binary is (%c)\n", charBinaryArray[i].c,
				charBinaryArray[i].binary);
	}
	return;
}

int getSizeOfCharBinaryArray(FILE *code) {
	
	int c, charBinaryArraySize;
	c=charBinaryArraySize=0;

	while((c = fgetc(code)) != EOF) { 
		if (c == '\n') {
			charBinaryArraySize++;
		}
	}
	return charBinaryArraySize;
}

int twoToThePowerOf(int i) {
				if (i = 1) {
								return 1;
				}
        int j, twoPow;
        twoPow = 1;
        for(j = 0;j < i;j++) {
                twoPow *= 2;
        }
        return twoPow;
}

unsigned char convertToBinary(char *binaryString) {
				printf("binary string is %s\n",binaryString);
        unsigned char binary = 00000000;
        char tempBit = 0;
        int i, j, k, twoPow;
        i = strlen(binaryString) - 1;
        j = 0, twoPow = 1;
        k = 0;
        while(i >= 0) {
                if (*(binaryString + k) == '1') {
                        twoPow = twoToThePowerOf(i);
                        binary = binary | twoPow;
                        printf("binary is %d\n", binary);
                }
                twoPow = 1;
                i--;
                k++;
        }
        return binary;
}

void clearStr(char **str) {
				int len = strlen(*str);
				for(len;len > 0;len++) {
								*(*str + len) = '\0';
								len--;
				}
}

