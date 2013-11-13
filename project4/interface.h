/**
 * interface.h
 *
 * Defines whatever is necessary to ensure the compatibility with the Autograder.
 * DO NOT upload it to Autograder, or your code cannot be graded.
 */

#ifndef INTERFACE_H_

	#define INTERFACE_H_

	#include <stdio.h>
	#include <stdarg.h>
	#include <sys/types.h>
	#include <sys/sem.h>

	#ifndef MAX_PROCS
		#define MAX_PROCS 3				/* maximum number of processes */
	#endif

	#ifndef MAX_BUFFER_SIZE
		#define MAX_BUFFER_SIZE 256		/* maximum  buffer size */
	#endif

	/**
	 * Defining wrapper functions.
	 */
	#define FORK fork
	#define SEMGET semget
	#define SHMGET shmget

	
	/**
	 * Struct to keep track of the relation between the process and
	 * the keywords frequency
	 */
	struct proc_key_count {
		pid_t pid;			/* to record process id */
		int key_id;			/* index of the keyword in the keyword list*/
		int freq;			/* number of occurrence. If no match found the result should be zero */
	}__attribute__((__packed__)); //struct

	
	extern int keys_cnt;		/* the total number of keywords*/
	
	extern char** keywords;		/* array of strings of keywords */
	
	extern int shm_id;			/* shared memory id*/
	
	extern void* shared_mem;	/* points to the beginning address of the shared memory to store results */

	/**
	* Checks the results written to the shared memory.
	*/
	void shmem_checker() {}

#endif /* INTERFACE_H_ */
