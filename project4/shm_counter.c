/**
 * shm_counter.c
 *
 * Additional Notes:
 *  1. Only the parent process is allowed to call FORK().
 *  2. Each child searches part of the input file for the keywords, and
 *     writes struct proc_key_count to the shared memory pointed to by shared_mem.
 */

#define MAXKEYSIZE 50

#include <stdio.h>
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
 
#include "interface.h"
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

int main(int argc, char **argv) {

	/*Args processing*/

	if (argc != 7) {
		printf("invalid number of args\n");
		return 1;
	}

	char *input, *output, *keyword;
	int bufferSize;

	int foundInput, foundOutput, foundKeyword, foundBufferSize;
	foundInput=foundOutput=foundKeyword=foundBufferSize=0;

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

	keywords = malloc(sizeof(char *)*MAXKEYWORDS);

	/*Initialize keyword array*/

	FILE *keywordFile = fopen(keyword,"r");
	fillKeyArray(&keywords, keywordFile);

	/*Initialize file streams*/

	FILE *inputFile = fopen(input, "r");
	FILE *outputFile = fopen(output, "w");

	/*free memory*/

	fclose(keywordFile);
	fclose(inputFile);
	fclose(outputFile);

	destroy_sharedmem();

	return 0;
}

void fillKeyArray(char ***keywords, FILE *keywordFile) {
	char c;
	int i, j; 
	i=j=0;
	while((c = fgetc(keywordFile)) != EOF) {
		if (isValidC(c)) {
			(*keywords)[i] = malloc(sizeof(char)*MAXKEYSIZE);
			while(isValidC(c)) {
				(*keywords)[j] = c;
				keys_cnt++;
				j++;
				c = fgetc(keywordFile);
			}
			i++;
		}
		c = fgetc(keywordFile);
	}
}

int isValidC(char c) {
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == ',' 
			|| c =='.' || c == ';' || c == '!';
}
