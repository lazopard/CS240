#include <stdio.h>

int is_alphanumeric(char c) {
	return c >= 'A' && c <='Z' || c >= 'a' && c<='z' || c >= '0' && c <='9';
}
