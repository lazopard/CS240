#include <stdio.h>
#include <dirent.h>

int main(void) {
	DIR *testdir = opendir(".");
	struct dirent *tempFile;
	while((tempFile = readdir(testdir))) {
		printf("%s\n", tempFile->d_name);
	}
	return 0;
}
