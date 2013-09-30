#include <stdio.h>

/**
 * Test if {ch} is a letter (a - z or A - Z).
 * Return 0 for letter, 1 for digit, 2 for whitespace and 3 for any other character.
 */
int isAlpha(char c) {
	return (c >= 'A' && c <= 'Z' || c >= 'a' &&  c <= 'z');
}

int isNumeric(char c) {
	return (c >= '0' && c  <= '9');
}

int getCharType(char ch) {
	//TODO: Finish this function
	if (isAlpha(ch))
		return 0;
	if (isNumeric(ch))
		return 1;
	if (ch == ' ' || ch == '\t' || ch == '\n')
		return 2;
	else
		return 3;
}

/**
 * Return the lower case equivalent of {c} if it 
 * is in upper case. Return {c} if it is already 
 * in lower case.
 */
char lowerCaseOf(char c){
	//TODO: Finish this function
	if (c >= 'A' && c <= 'Z')
		return c +32;
	return c;
}

/**
 * Return the length of the zero terminated string {s}.
 * A null terminated string is an array of characters 
 * with a \0 character at the end.
 * The \0 character is not counted, e.g. the length of "abc" is 3.
 */
int strLen(char s[]) {
	//TODO: Finish this function
	int len=0;
	while(s[len] != '\0')	
		len++;
	return len;
}

/**
 * Compare the first {n} characters of string {s1} and {s2}.
 * If identical, return 1. Otherwise return 0.
 */
int strNCmp(char s1[], char s2[], int n) {
	//TODO: Finish this function
	int i=0;
	for(i; i<n; i++)
		if (!(s1[i] == s2[i]))
			return 0;
	return 1;
}

/**
 * Replace every character of {s} with {c}.
 * {len} indicates the length of {s}.
 */
void replaceChar(char s[], char c, int len) {
	//TODO: Finish this function
	int i=0;
	for(i; i<len;i++)
		s[i] = c;
}
