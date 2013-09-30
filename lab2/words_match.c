#include <stdio.h>

int match(char swear_word[], char message_word[], int length) {
	int i,count = 0;
	for(i;i<length;i++)
		if(message_word[i] == swear_word[i]) 
			count++;
	return (count == length);
}
