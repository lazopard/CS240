#define INPUT "-i"
#define OUTPUT "-o"
#define BUFFERSIZE "-b"
#define KEYWORD "-k"

#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

	
	int pipe_fds[2];
	int read_fd;
	int write_fd;
	pipe(pipe_fds);
	read_fd = pipe_fds[0];
	write_fd = pipe_fds[1];
	int count = 0;

	/*word counting starts*/

	int i;
	pid_t cpid;

	for(i = 0; i < 20; i++) {
		cpid = fork();
		if (cpid == 0) {
			count++;
		}
		else {
		}
	}

	/*word counting ends*/

	printf("count is %d\n",count);

	return 0;
}
