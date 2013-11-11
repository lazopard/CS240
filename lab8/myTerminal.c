#define MAXBUFFSIZE 120

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myTerminal.h"
#include <sys/stat.h> 
#include <fcntl.h>

void execCommand(int argc, char** argv, int fdWrite) {

}

int main(int argc, char **argv) {

	if (argc != 2) {
		perror("invalid number of arguments\n");
		return 1;
	}

	int fd  = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("error opening file\n");
		return 1;
	}
	char *buffer = malloc(sizeof(char)*MAXBUFFSIZE);
	int n = read(fd, buffer, MAXBUFFSIZE);
	char **commands = malloc(sizeof(char)*n);
	buffer = strtok(buffer, " \n");
}
