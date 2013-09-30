#include <stdarg.h>
/**
 * fileIO.h
 *
 * Header to run supplementary library libfileIO.
 */
 
 #define KEEP_CONTENT 1
 #define DISCARD_CONTENT 0

/**
 * Read the specified file to a string array and return the pointer.
 * The string ends with the '\0' terminator.
 * The variable pointed to by size will be updated with the size of the file.
 * Return NULL if any error happens loading the file.
 * 
 * Usage Example: unsigned char* ptr = readFile("sample.txt", &s);
 */
unsigned char* readFile(const char *fileName, int *size);

/**
 * print the formatted string to a file.
 * oflag == KEEP_CONTENT : keep current content, if any, of the file
 * oflag == DISCARD_CONTENT : make the file empty before printing the data
 * Return 0 if the data is successfully written to the file.
 * Return 1 if any error happens.
 */
int printFile(const char* fileName, int oflag, char* format, ...);

/**
 * Free the allocated memory
 */
int freeMem(void);
