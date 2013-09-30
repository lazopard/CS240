/* Leonardo Azopardo 
 * PUID:002602680
 * email:lazopard@purdue.edu
 *
 * CS240 Project 1
 *
 */

#define SHIFT 1 
#define SUBSTITUTION 2 
#define FREQUENCY 3 
#define BRUTE_FORCE 4
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cipher.h"
#include "fileIO.h"


//MAIN FUNCTION

int argumentsAreValid(int foundInput, int foundOutput,
						int foundEncription, int foundDecription);

void shift_enc(unsigned char *input, int key, unsigned char *output);

void shift_dec(unsigned char *input, int key, unsigned char *output);

void shift_bf(unsigned char *input, const char *outFilename); 

void subst_enc(unsigned char *input, unsigned char *alphabet, unsigned char *key, unsigned char *output); 

void subst_dec(unsigned char *input, unsigned char *alphabet, unsigned char *key, unsigned char *output);

void analyzeFreq(unsigned char *input, const char *outFilename); 

int main(int argc, char **argv) {
	int i,size,foundInput,shiftN,foundOutput,option,foundEncription,foundDecription;
	i=foundInput=size=shiftN=foundOutput=foundEncription=foundDecription = 0;
	char *inputFile = NULL;
	char *outputFile = NULL;
	
	unsigned char *inFile = NULL;
	char *input = "-i";
	char *output = "-o";
	char *decription = "-d=";
	char *encription = "-e=";
	char *substitution = "subst";
	char *frequency = "freq";
	char *shift = "shift";
	for(i = 1; i < argc; i++) {
		if (!strcmp(argv[i], input)) {
			foundInput++;
			inputFile = argv[i+1];
		}
		if (!strcmp(argv[i], output)) {
			foundOutput++;
			outputFile = argv[i+1];
		}
		if (!strncmp(argv[i],decription,2)) {
			foundDecription++;
			if (strstr(argv[i],shift)) {
				if (atoi(argv[i+1])) {
					shiftN = atoi(argv[i+1]);
					option = SHIFT;
				}
				else { 
					option = BRUTE_FORCE;
				}
			}
			else if (strstr(argv[i],frequency)) 
				option = FREQUENCY;
			else if (strstr(argv[i],substitution))
				option = SUBSTITUTION;
			else {
				printf("Invalid argument usage.\n");
				return 1;
			}
		}
		if (!strncmp(argv[i],encription,2)) {
			foundEncription++;	
			if (strstr(argv[i],shift)) {
				option = SHIFT;
				if (atoi(argv[i+1]))
					shiftN = atoi(argv[i+1]);
				else 
					shiftN = DEFAULT_SHIFT;
			}
			else if (strstr(argv[i],substitution))
				option = SUBSTITUTION;
			else {
				printf("Invalid argument usage.\n");
				return 1;
			}

		}
	}
	if (argumentsAreValid(foundInput,foundOutput,foundEncription,foundDecription)) {
		if (!(inFile = readFile(inputFile, &size))) {
			printf("Error loading input file.\n");
			return 1;
		}
		
		else if (foundEncription) {
			if (option == SHIFT) {
				char encriptionOutput[size];
				shift_enc(inFile,shiftN,encriptionOutput);
				printFile(outputFile,DISCARD_CONTENT,encriptionOutput);
			}
			else if (option == SUBSTITUTION) {
				char encriptionOutput[size];
				subst_enc(inFile, ALPHABET, SUBSTITUTION_KEY, encriptionOutput); 
				printFile(outputFile,DISCARD_CONTENT,encriptionOutput);
			}
			else {
			}
		}

		else {
			if (option == SHIFT) {
				char decriptionOutput[size];
				shift_dec(inFile,shiftN,decriptionOutput);
				printFile(outputFile,DISCARD_CONTENT,decriptionOutput);
			}	
			else if (option == BRUTE_FORCE) {
					shift_bf(inFile, outputFile);
			}
			else if (option == SUBSTITUTION) {
				char decriptionOutput[size];
				subst_dec(inFile, ALPHABET, SUBSTITUTION_KEY, decriptionOutput); 
				printFile(outputFile,DISCARD_CONTENT,decriptionOutput);
			}
			else if (option == FREQUENCY) {
				analyzeFreq(inFile, outputFile);
			}
			else {
			}
		}

		freeMem();
		return 0;
	}

	else  {
		printf("Invalid argument usage.\n");
		return 1;
	}
}

//helper function to check if the command-line arguments are valid, returns 1 if they are, 0 otherwise
int argumentsAreValid(int foundInput, int foundOutput,
						int foundEncription, int foundDecription) {
	if (foundInput >= 2 || foundOutput >= 2 || foundEncription >= 2 || foundDecription >= 2)
		return 0;
	if (!(foundInput || foundOutput))
		return 0;
	if ( (foundDecription && foundEncription) || (!(foundDecription || foundEncription))) 
		return 0;
	return 1; 	
}

/* Shift encription and Decription
*
*
*
*
*
*/

//helper functin that converts a whole string to uppercase

void stringToUpper(char *string);

//helper function that finds the index of a character in an alphabet
int findIndex(unsigned char *key, char c, int len);

void shift_enc(unsigned char *input, int key, unsigned char *output) {
	stringToUpper(input); 
	int i,j,len,indexInAlpha; 
	j = 0;
	len = strlen(input); 
	for(i=0;i < len;i++) {
		indexInAlpha = findIndex(ALPHABET, input[i] , SIZE_OF_ALPHABET);
		if (indexInAlpha != -1) {
			indexInAlpha = indexInAlpha + key;
			if (indexInAlpha < 0) {
				indexInAlpha = indexInAlpha + (2*SIZE_OF_ALPHABET);
			}
			output[j] = ALPHABET[indexInAlpha % SIZE_OF_ALPHABET];
			j++;
		}
		else {
			if (input[i] == '\n') {
				output[j] = '\n';
				j++;
			}
		}
	}
	output[j] = '\0';
}
	

void shift_dec(unsigned char *input, int key, unsigned char *output) {
	stringToUpper(input); 
	int i,j,len,indexInAlpha; 
	j = 0;
	len = strlen(input); 
	for(i=0;i < len;i++) {
		indexInAlpha = findIndex(ALPHABET, input[i] , SIZE_OF_ALPHABET);
		if (indexInAlpha != -1) {
			indexInAlpha = indexInAlpha - key;
			if (indexInAlpha < 0) {
				indexInAlpha = indexInAlpha + (2*SIZE_OF_ALPHABET);
			}
			output[j] = ALPHABET[indexInAlpha % SIZE_OF_ALPHABET];
			j++;
		}
		else {
			if (input[i] == '\n') {
				output[j] = '\n';
				j++;
			}
		}
	}
	output[j] = '\0';
}

//BRUTE FORCE ATTACK
//
//

void shift_bf(unsigned char *input, const char *outFilename) {
	stringToUpper(input);
	int i,j,len,indexInAlpha;
	len = strlen(input);
	char output[sizeof(input) + 1];
	printFile(outFilename,DISCARD_CONTENT,"");
	for(i=1; i < SIZE_OF_ALPHABET; i++) {
		shift_dec(input,i,output);
		printFile(outFilename,KEEP_CONTENT,"%s",output);
	}
}

//Substitution encription and decription
//
//

//counts the number of letters in a string
int countLetters(char *string,int len);

void subst_enc(unsigned char *input, unsigned char *alphabet, unsigned char *key, unsigned char *output) {
	stringToUpper(input);
	int i,letterCount,j,input_len,key_len,indexInKey;
	j=0;
	input_len = strlen(input);
	key_len = strlen(key);
	for(i = 0; i < input_len;i++) {
		indexInKey = findIndex(alphabet, input[i],key_len);
		if (indexInKey != -1) {
			output[j] = key[indexInKey];
			j++;
		}
		else {
			if (input[i] == '\n') {
				output[j] = input[i];
				j++;
			}
		}
	}
	output[j] = '\0';
}

void subst_dec(unsigned char *input, unsigned char *alphabet, unsigned char *key, unsigned char *output) {
	stringToUpper(input);
	int i,j,input_len,key_len,alphabet_len,indexInAlpha;
	j = 0;
	input_len = strlen(input);
	key_len = strlen(key);
	alphabet_len = strlen(alphabet);
	for(i = 0; i < input_len;i++) {
		indexInAlpha = findIndex(key,input[i],alphabet_len);
		if (indexInAlpha != -1) {
			output[j] = ALPHABET[indexInAlpha];
			j++;
		}
		else {
			if(input[i] == '\n') {
				output[j] = input[i];
				j++;
			}
		}
	}	
	output[j] = '\0';
}

/**
 * Frequency Analysis
 *
 * outFilename - the file to write output to.
 * Input data ends with a ‘\0’ terminator.
 */

/* structure that contains a character and a 
 * double representing its frequency
 */

typedef struct {
	char c;
	double freq;
} charFreq;

void stringToUpper(char *string);

//compares the value of two doubles
int compareDouble(double a, double b);

//sorts a charFreq element by its frequency
void charFreqSort(charFreq *charsFreq, int len);

void analyzeFreq(unsigned char *input, const char *outFilename) {
	
	int i,len,j,letterCount;
	double frequency;
	stringToUpper(input);
	charFreq charsFreq[SIZE_OF_ALPHABET]; charFreq stdFreq[SIZE_OF_ALPHABET];
	for(i=0;i < SIZE_OF_ALPHABET;i++) {
		charsFreq[i].freq = 0.0;
		charsFreq[i].c = ALPHABET[i];
		stdFreq[i].freq = freq[i];
		stdFreq[i].c = ALPHABET[i];
	}
	len = strlen(input);
	letterCount = countLetters(input,len);
	for(i=0;i < len-1; i++) {
		for(j=0;j < SIZE_OF_ALPHABET;j++) {
			if (*(input + i) == ALPHABET[j]) {
				charsFreq[j].freq = charsFreq[j].freq + 1.00;
			}
		}
	}
	for(i=0;i < SIZE_OF_ALPHABET; i++) {
		frequency = (charsFreq[i].freq)/((double)(letterCount));
		charsFreq[i].freq = frequency;
	}
	charFreqSort(charsFreq,SIZE_OF_ALPHABET);
	charFreqSort(stdFreq,SIZE_OF_ALPHABET);
	printFile(outFilename, DISCARD_CONTENT, "Frequency\t\t\tLetter\t\t\tPredict\t\t\tStd Freq\n");
	for(i = 0;i < SIZE_OF_ALPHABET;i++) {
		printFile(outFilename, KEEP_CONTENT, "%.6f\t\t\t%c\t\t\t%c\t\t\t%.6f\n",
			charsFreq[i].freq,charsFreq[i].c,stdFreq[i].c,stdFreq[i].freq);
	}
	printFile(outFilename,KEEP_CONTENT,'\0');
}

void charFreqSort(charFreq *charsFreq, int len) {
	//Not the fastest but good enough for only 26 elements
	int i,j;
	charFreq temp;
	for(i=0; i < len; i++) {
		for(j=0;j < len;j++) {
			if (compareDouble(charsFreq[i].freq, charsFreq[j].freq)) {
				temp.c = charsFreq[i].c;
				temp.freq = charsFreq[i].freq;
				*(charsFreq + i) = *(charsFreq + j);
				*(charsFreq + j) = temp;
			}
		}
	}
}

int compareDouble(double a, double b) { 
	if ((a - b) > 0.0 )
		return 1;
	else if ((a-b) == 0.0)
		return -1;
	else
		return 0;
}

int findIndex(unsigned char *key, char c, int len) {
	int i;
	for(i = 0; i< len; i++) {
		if (c == key[i]) {
			return i;
		}
	}
	return -1;
}

char toUpper(char c) {
	if (c >= 'a' && c <= 'z')
		return c - 32;
	return c;
}

void stringToUpper(char *string) {
	int i, len;
	len = strlen(string);
	for(i=0; i<len; i++) {
		*(string + i) = toupper(*(string+i));
	}
}

int countLetters(char *string,int len) {
	int count,i;
	count=i=0;
	for(i; i < len; i++) {
		if ( *(string + i) >= 'A' && *(string + i) <= 'Z' || *(string + i) >= 'a' && *(string + i) <= 'z')
			count++;
	}
	return count;
}
