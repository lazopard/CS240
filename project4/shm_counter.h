/**
 * shm_counter.h
 *
 * You may add your own definitions and declarations in this file.
 */

#ifndef WORKERS_H_

	#include "interface.h"

	/**
	 * Define your variables, functions, structs etc., here if you need.
	 */

	#define WORKERS_H_
	#define MAXKEYSIZE 50
	#define KEYLENGTH 20
	#define INPUT "-i"
	#define OUTPUT "-o"
	#define BUFFERSIZE "-b"
	#define KEYWORD "-k"

	int isValidC(char c);

	int fillKeyArray(char ***keywords, FILE *keywordFile);

#endif /* WORKERS_H_ */
