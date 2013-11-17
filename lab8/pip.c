#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	int pipefd[2];
	pid_t cpid;
	char buf;


