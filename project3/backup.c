/*dir ops
 * scandir
 * rmdir
 * opendir
 * readdir
 * mkdir
 * ///
 * file ops
 * fopen
 * fread/fwrite
 * fprintf
 */


#define SOURCE "-s"
#define DESTINATION "-d"
#define MAXBACKUPS "-m"
#define NTYPES 2

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "string.h"
#include "backup.h"
#include <dirent.h>
#include <assert.h>

int main(int argc, char **argv) {
	
	/* Arguments processing */
	
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

	/* Arguments processing end */
	


	return 0;
}



char *getFStats(char *fileName) {
	struct stat fileStats;
	assert(!stat(fileName, &fileStats));
	int i;

	// get type 
	const char *type;
	for(i = 0;i < NTYPES;i++) {

		if(S_ISDIR(fileStats.st_mode))
			type = "DT_DIR";
		else if(S_ISREG(fileStats.st_mode)) 
			type = "DT_REG";
		else {
			fprintf(stderr,"Unknown file type\n");
			abort();
		}
	}

	//get size in bytes
	int size = fileStats.st_size;

	//get creation time
	
	struct tm *creationTime;

	//get last mod time
	
	//get dataName
	


	return "Incomplete function\n";
}

	

void createLog(char *sourceDir, char *logFilePath) {
	DIR *dir = opendir(sourceDir);
	assert(dir != NULL);
	FILE *newLog = fopen(logFilePath, "w");
	assert(newLog != NULL);

	

	fclose(newLog);
	closedir(dir);
	return;
}

int compareLog(FILE *oldLogFile, FILE *newLogFile) { /* not tested */
	int c1,c2;
	while((c1 = fgetc(oldLogFile)) == (c2 = fgetc(newLogFile)) && (c1 != EOF && c2 != EOF));
	if (c1 == c2)  
		return 1;
	else
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
