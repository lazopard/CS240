/**
 * tester.c
 *
 * This file is used to demonstrate how to unit test your program.
 * You are not required to finish and submit this.
 */

#include <stdio.h>
#include "utils.h"

void testgetCharType() {
	if(getCharType('a') != 0)
		printf("getCharType('a') does not return 0\n");
	if(getCharType('9') != 1)
		printf("getCharType('9') does not return 1\n");
	// more Cases ...
}

void testToLower() {
	printf("the lower Case of A is %c\n",lowerCaseOf('A')); 
	printf("the lower Case of a is %c\n",lowerCaseOf('a'));
	printf("the lower Case of 0 is %c\n",lowerCaseOf('0'));
}

void teststrLen() {
	printf("len of abc is %d\n",strLen("abc"));
	printf("len of a is %d\n",strLen("a"));
	printf("len of abc + newline is %d\n",strLen("abc\n"));
}

void teststrNCmp() {
	printf("abc to bc, %d\n",strNCmp("abc","bc",2));
	printf("abc to abc, %d\n",strNCmp("abc","abc",3));
	printf("abc to bc, %d\n",strNCmp("abc","abc",2));
	printf("abc to bc, %d\n",strNCmp("abc","a",1));
}

void testreplaceChar() {
	char s1 [] = "abc";
	replaceChar(s1,'a',3);
	printf("replace abc with a,,,,%s\n",s1);
}
int main(){
	testgetCharType();
	testToLower();
	teststrLen();
	teststrNCmp();
	testreplaceChar();
}
