#define BUFFERSIZE 10000
#define INPUTFILE "-ib"
#define OUTPUTFILE "-o"
#define CODEFILE "--code"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct charByte {
				unsigned char byte;
				char c;
};

void printCharByteArray(struct charByte[], int size);

int main(int argc, char **argv) {
				int i, charCode, decode;
				i=charCode=decode=0;
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
				FILE *code = fopen(outputFile, "r");
				if (code == NULL) {
								printf("File %s does not exist.\n", codeFile);
								return 1;
				}

				int c, charByteArraySize;

				while((c = fgetc(code)) != EOF) { 
								if (c == '\n') {
												charByteArraySize++;
								}
				}
				charByteArraySize++;

				rewind(code);  

				struct charByte charByteArray[charByteArraySize];

				//populate charByteArray

				i = 0, c = 0;
				while((c = fgetc(code)) != EOF) {
								if (c == '=') {
												charByteArray[i].c = ungetc(c,code);
												//previous of c is char
												//after c is binary
												charByteArray[i].byte = fgetc(code);
												i++;
								}
				}

				int sizeOfWrite, j;
				j = 0;

				//encode input
				if (!decode) {
								while((c = fgetc(input)) != EOF) {
												for(i = 0; i < charByteArraySize; i++) {
																if (c == charByteArray[i].c) {
																				buffer[j] = charByteArray[i].byte;
																				j++;
																}	
												}
								}
								//deal with padding 0s
								unsigned char padding = 00000000;
								buffer[j + 1] = padding;
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


void printCharByteArray(struct charByte charByteArray[], int size) {
				int i;
				for(i = 0; i < size; i++) {
								printf("charByteArray[i].c is %c, charByteArray[i].byte is %u\n", charByteArray[i].c,
																charByteArray[i].byte);
				}
				return;
}

