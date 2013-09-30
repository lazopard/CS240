#define MAXLENGTH 140
#define SWEARWORDNUM 5
#define SWEARWORDLEN 4
#include <stdio.h>

int is_alphanumeric(char c);
int match(char swear_word[], char message_word[], int length);
int byItself(char message[], int position, int length);

int main() {
	char message[MAXLENGTH];
	char swear_word[SWEARWORDLEN];
	char swear_words[5][4];
	swear_words[0][0] = 'h';
	swear_words[0][1] = 'e';
	swear_words[0][2] = 'c';
	swear_words[0][3] = 'k';
	swear_words[1][0] = 'c';
	swear_words[1][1] = 'r';
	swear_words[1][2] = 'a';
	swear_words[1][3] = 'p';
	swear_words[2][0] = 's';
	swear_words[2][1] = 'u';
	swear_words[2][2] = 'c';
	swear_words[2][3] = 'k';
	swear_words[3][0] = 'b';
	swear_words[3][1] = 'u';
	swear_words[3][2] = 'l';
	swear_words[3][3] = 'l';
	swear_words[4][0] = 'd';
	swear_words[4][1] = 'r';
	swear_words[4][2] = 'a';
	swear_words[4][3] = 't';
	int i,count,j,k;
	char c;
	count = 0;
	while ((c = getchar()) != EOF && count < MAXLENGTH) {
		message[count] = c;
		count++;
	}
	for(i=0;i < count;i++) 
		for(j=0;j<SWEARWORDNUM;j++) 	
			if ((message[i] == swear_words[j][0]) && byItself(message,i,SWEARWORDLEN)) {
				for(k=0;k < SWEARWORDLEN;k++)
					swear_word[k] = message[i+k];
				if (match(swear_words[j],swear_word,SWEARWORDLEN));
					for(k=0;k<SWEARWORDLEN;k++)
						message[i+k] = 'X'; 
			}
	printf("%s",message);
}

int byItself(char message[],int position, int length) {
	return (!(is_alphanumeric(message[position-1])) && !(is_alphanumeric(message[position+length-1])));
}
int match(char swear_word[], char message_word[], int length) {
	int i,count;
	for(i=0;i<length;i++)
		if(message_word[i] == swear_word[i]) 
			count++;
	printf("got to match");
	return (count == length);
}

int is_alphanumeric(char c) {
    return c >= 'A' && c <='Z' || c >= 'a' && c<='z' || c >= '0' && c <='9';
}
