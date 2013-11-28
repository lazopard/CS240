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

	if (shmdt(shared_mem) == -1) { /*detach shared_mem*/
		perror("shmdt failed...\n");
	}

	struct shmid_ds shmid_struct;
	if (shmctl(shm_id, IPC_RMID, &shmid_struct) == -1) { /*remove memory id*/
		perror("shmctl failed...\n");
		exit(EXIT_FAILURE);
	}
}

int isValidC(char c);

int fillKeyArray(char ***keywords, FILE *keywordFile);

void printProcKeyCount(struct proc_key_count keyCount, char **keywords, FILE *outputStream);

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

	/*Read from input*/

	FILE *inputFile = fopen(input, "r");
	fseek(inputFile, 0, SEEK_END);
	int fileSize = ftell(inputFile);
	rewind(inputFile);
	char *buffer = malloc(sizeof(char)*size + 1);
	fread(buffer, sizeof(char), fileSize, inputFile);
	fclose(inputFile);

	/*Initialize keyword array*/

	FILE *keywordFile = fopen(keyword,"r");
	keywords = malloc(sizeof(char *)*MAXKEYSIZE);
	keys_cnt = fillKeyArray(&keywords, keywordFile);
	fclose(keywordFile);

	/*Initialize multi-process variables and shared memory*/

	shm_id = SHMGET(IPC_PRIVATE, sizeof(struct proc_key_count *),IPC_CREAT | IPC_EXCL | 0666);
	if (shm_id == -1) {
		perror("SHMGET failed\n");
		return 1;
	}

	shared_mem = (struct proc_key_count **) shmat(shm_id,NULL,0);
	if (shared_mem == NULL) {
		perror("SHMAT failed\n");
		return 1;
	}

	int semid = SEMGET(IPC_PRIVATE, MAX_PROCS, IPC_CREAT | IPC_EXCL | 0666);

	if (semid == -1) {
		perror("semget failed...\n");
		return 1;
	}

	/*Counting starts */

	for(i = 0; i < MAX_PROCS;i++) {

		cCount=childCount=0;

		if (pipe(pipe_fds) == -1) {
			perror("pipe failed...\n");
			exit(EXIT_FAILURE);
		}

		cpid = fork();

		if (cpid < 0) { /*if ford failed*/
			perror("fork failed...\n");
			return 1;
		}
		else if (cpid == 0) { /*child process*/
			close(pipe_fds[0]);
			char *tempString = malloc(sizeof(char)*fileSizePerChild);
			fseek(inputFile,i*fileSizePerChild, SEEK_SET);
			fread(tempString, sizeof(char), fileSizePerChild, inputFile);
			childCount = countWords(tempString, keyword);
			write(pipe_fds[1], &childCount, sizeof(int));
			free(tempString);
			_exit(0);
		}
		else { /*parent process*/
		}
	}	

	/*Counting ends*/

	/*Write results to ouputFile*/

	FILE *ouputFile = fopen(output);

	for(i = 0; i < keys_cnt; i++) {
		printProcKeyCount(*(shared_mem[0]),keywords, outputFile);
	}
	fclose(outputFile);

	/*free memory, destroy shared mem...*/

	free(buffer);

	destroy_sharedmem();

	return 0;
}

/*checks if C is a valid character for a keyword as specified in the handout*/

int isValidC(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == ',')
		|| (c =='.') || (c == ';') || (c == '!') ;
}

/*checks if the line contains a valid keyword as specified in the handout*/

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

/*Fills the empty char* array KEYWORDS with the keywords in KEYWORDFILE*/

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
	return i;
}  

/*checks if C is a delimiter as specified in the handout*/

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

/*prints to outputStream with specified format*/

void printProcKeyCount(struct proc_key_count keyCount, char **keywords, FILE *outputStream) {
	fprintf(outputStream, "%s: %d\n",keywords[key_id], keyCount.freq);
}
