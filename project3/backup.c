#define SOURCE "-s"
#define DESTINATION "-d"
#define MAXBACKUPS "-m"

#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "backup.h"

int main(int argc, char **argv) {
	
	//Arguments processing
	
	char *sourceDir, *destDir;
	int foundS, foundDest, foundMaxB, i, maxB;

	foundS=maxB=foundDest=foundMaxB=0;

	for(i=0; i < argc;i++) {
		if (!strcmp(argv[i],SOURCE)) {
			sourceDir = argv[i+1];
			foundS++;
			i++;
		}
		else if (!strcmp(argv[i],DESTINATION)) {
			destDir = argv[i+1];
			foundDest++;
			i++;
		}
		else if (!strcmp(argv[i],MAXBACKUPS)) {
			maxB = atoi(argv[i+1]);
			foundMaxB++;
			i++;
		}
		else {
			fputs("Usage:\n./backup ‐s sourceDir [‐d destinationDir ‐m X]\n",stderr);
			return 1;

		}
	}

	if (((argc-1) % 2 != 0) || !foundS) {
		fputs("Usage:\n./backup ‐s sourceDir [‐d destinationDir ‐m X]\n",stderr);
		return 1;
	}
	if (!foundDest) {
		destDir = DEFAULT_DEST_DIR;
	}
	if (!foundMaxB) {
		maxB = DEFAULT_MAX_BACKUPS;
	}

	//Arguments processing end
	


	return 0;
}

void createLog(char *sourceDir, char *logFilePath) {
	return;
}

int compareLog(FILE *oldLogFile, FILE *newLogFile) {
	return 0;
}

int copyFile(char *sourcePath, char *destinationPath) {
	return 0;
}

int copyDir(char *sourceDir, char *backupDir) {
	return 0;
}

int getNumOfBackup(char *destinationDir) {
	return 0;
}

int removeOldestBackup(char *destinationDir) {
	return 0;
}
