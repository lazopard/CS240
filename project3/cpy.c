#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int copyFile(char *sourcePath, char *destinationPath) {
	FILE *source = fopen(sourcePath, "r");
	assert(source != NULL);
	FILE *destination = fopen(destinationPath, "w");
	assert(destination != NULL);
	char c;
	while((c = fgetc(source)) != EOF) {
		if ((fputc(c,destination)) == EOF) {
			printf("write to %s failed, function copyFile", destinationPath);
			return 0;
		}
	}
	fclose(source);
	fclose(destination);
	return 1;
}

int compareLog(FILE *oldLogFile, FILE *newLogFile) {
	int c1,c2;
	while((c1 = fgetc(oldLogFile)) == (c2 = fgetc(newLogFile)) && (c1 != EOF && c2 != EOF));
	if (c1 == c2)  
		return 1;
	else
		return 0;
}

int main(void) {
	char *source = "backup.c";
	char *destination = "newBackup.c";
	(compareLog(fopen(source, "r"), fopen(destination, "r"))) ? printf("copyFile succeded\n") : printf("copyFile failed\n");
	 return 0;
}
