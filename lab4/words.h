#define SIZE 140

int readMsg(char* buf);

int getNextWordIndex(char* str, int len, int prevEnd);

void matchAndEraseWord(char* str, int len, int wLen, char* keyword);

void lowerStringCase(char* str, int len);

int wordLen(char* str, int strLen, int start);
