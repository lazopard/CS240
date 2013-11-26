/**
 * shm_counter.c
 *
 * Additional Notes:
 *  1. Only the parent process is allowed to call FORK().
 *  2. Each child searches part of the input file for the keywords, and
 *     writes struct proc_key_count to the shared memory pointed to by shared_mem.
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>			
#include <stddef.h>
#include <semaphore.h>

#include "shm_counter.h"

/**
 * Initialize the pre-defined global variables.
 * You must use them to be compatible with Autograder.
 * These variables are defined and described in interface.h.
 */
char**	keywords	= NULL;			/* array of keywords */
int		keys_cnt	= 0;			/* the total number of keywords */
int		shm_id		= 0;			/* shared memory id */
void*	shared_mem	= NULL;			/* pointer to shared memory returned by SHMGET */

/**
 * void destroy_sharedmem()
 *
 * Clear the shared memory and dynamic allocated memory, if any.
 */

void destroy_sharedmem() {

	/**
	 * Autograder function to check if all data stored in shared memory is correct.
	 * Do not free the memory before this function is called.
	 */
	shmem_checker();

	/**
	 * TODO: Write your code to clean all shared memory you created here.
	 */

} //destroy_sharedmem()

/**
 * TODO: Create whatever functions you need here.
 */

/**
 * TODO: Complete the main function.
 * Be sure to set keys_cnt, keywords, shm_id, and shared_mem to their actual values. 
 */

int isValidC(char c);

int fillKeyArray(char ***keywords, FILE *keywordFile);

int main(int argc, char **argv) {

	/*Args processing*/

	if (argc != 7) {
		printf("invalid number of args\n");
		return 1;
	}

	char *input, *output, *keyword;

	int i;

	for(i=1;i < argc;i++) {

		if (!strcmp(argv[i], INPUT)) {
			input = argv[i+1];
			i++;
		}
		else if (!strcmp(argv[i], OUTPUT)) {
			output = argv[i+1];
			i++;
		}
		else if (!strcmp(argv[i], KEYWORD)) {
			keyword = argv[i+1];
			i++;
		}
		else {
			printf("Invalid argument...\n");
			return 1;
		}
	}

	/*Args processing end*/


	/*Initialize keyword array*/

	keywords = malloc(sizeof(char *)*MAXKEYSIZE);
	FILE *keywordFile = fopen(keyword,"r");
	keys_cnt = fillKeyArray(&keywords, keywordFile);
	fclose(keywordFile);

	/*Initialize file streams*/

	FILE *inputFile = fopen(input, "r");
	FILE *outputFile = fopen(output, "w");

	/*Count words*/

	/*free memory, close streams*/

	fclose(inputFile);
	fclose(outputFile);

	destroy_sharedmem();

	return 0;
}

int isValidC(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == ',')
		|| (c =='.') || (c == ';') || (c == '!') ;
}

int isValidLine(char *string) {
	int i;
	if (isspace(string[0]))
		return 0;
	int len = strlen(string);
	for(i = 0; i < len; i++) {
		if (!isValidC(string[i]) && string[i] != '\n')
			return 0;
	}
	return 1;
}

int fillKeyArray(char ***keywords, FILE *keywordFile) {
	int i = 0;
	char *string = malloc(sizeof(char)*KEYLENGTH);
	while((string = fgets(string, KEYLENGTH, keywordFile)) != NULL) {
		if (isValidLine(string)) {
			(*keywords)[i] = malloc(sizeof(char)*strlen(string));
			strcpy((*keywords)[i], string);
			i++;
		}
		memset(string, '\0', KEYLENGTH);
	}
	free(string);
	return i - 1;
}  

int isDelimiter(char c) {
	return (c == ' ' || c == '\t' || c == '\n');
}

/*given a STRING count number of times KEYWORD appears*/

int countWords(char *string, const char *keyword) {
	char c;
	int keylen = strlen(keyword);
	int i = 0;
	int matchlen, count;
	count = 0;
	if (!strncmp(string,keyword,sizeof(char)*keylen)
			&& isDelimiter(*(string + keylen))) {
		count++;
		i += keylen;
	} 
	while((c = string[i]) != EOF) {
		matchlen = 0;
		if (c == keyword[0] && isDelimiter(*(string + i - 1))
				&& isDelimiter(*(string + i + keylen))) {
			while((c = string[i + matchlen]) == keyword[matchlen])
				matchlen++;
			if (matchlen == keylen) {
				count++;
			}
			i += matchlen;
		}
		else 
			i++;
	}
	return count;
}
