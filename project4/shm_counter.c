/**
 * shm_counter.c
 *
 * Additional Notes:
 *  1. Only the parent process is allowed to call FORK().
 *  2. Each child searches part of the input file for the keywords, and
 *     writes struct proc_key_count to the shared memory pointed to by shared_mem.
 */

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

int main() {
	
	/**
	 * TODO: Complete the main function.
	 * Be sure to set keys_cnt, keywords, shm_id, and shared_mem to their actual values. 
	 */

	destroy_sharedmem();
	
	return 0;
} //main()
