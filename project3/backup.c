#define SIZEOFDATE 19
#define MAXFORMATSIZE 120
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

const char *weekdays[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
								"Oct", "Nov", "Dec"};
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
			fputs("Usage:\n./backup -s sourceDir [-d destinationDir -m X]\n",stderr);
			return 0;

		}
	}

	if (((argc-1) % 2 != 0) || !foundS) {
		fputs("Usage:\n./backup -s sourceDir [-d destinationDir -m X]\n",stderr);
		return 0;
	}
	if (!foundDest) {
		destDir = DEFAULT_DEST_DIR;
	}
	if (!foundMaxB) {
		maxB = DEFAULT_MAX_BACKUPS;
	}

	/* Arguments processing end */

	char *logFilePath = malloc(sizeof(char)*strlen(destDir) +
			sizeof(char)*strlen(LOG_NEW_FILENAME) + 1);
	
	logFilePath = destDir;
	logFilePath = strcat(logFilePath, "/");
	logFilePath = strcat(logFilePath,LOG_NEW_FILENAME);
	createLog(sourceDir,logFilePath);
	char *oldLogFilePath = malloc(sizeof(char)*strlen(destDir) +
			sizeof(char)*strlen(LOG_LAST_FILENAME) + 1);
	oldLogFilePath = destDir;
	oldLogFilePath = strcat(oldLogFilePath, "/");
	oldLogFilePath = strcat(oldLogFilePath,LOG_LAST_FILENAME);
	FILE *newLog = fopen(logFilePath,"r");
	assert(newLog != NULL);
	FILE *oldLog = fopen(oldLogFilePath,"r");
	assert(oldLog != NULL);

	if (!compareLog(newLog, oldLog)) {
		return 1;
	}

	else {
		//get date and build destDir full path
		if (!copyDir(sourceDir,destDir)) {
			perror("Copying the directory failed.\n");
			return 0;
		}
		//update name of newlog to oldlog
	}

	if (getNumOfBackup(destDir) > maxB) {
		if(!removeOldestBackup(destDir)) {
			perror("Removing oldest backup failed.\n");
			return 0;
		}
	}

	fclose(newLog);
	fclose(oldLog);

	return 1;
}



void putFStats(char *fileName, char **buf) { //not tested

	struct stat fileStats;
	assert(!stat(fileName, &fileStats));
	int i;
	const char *type;

	for(i = 0;i < NTYPES;i++) {

		if(S_ISDIR(fileStats.st_mode))
			type = "DT_DIR";
		else if(S_ISREG(fileStats.st_mode)) 
			type = "DT_REG";
		else {
			perror("Unknown file type\n");
			abort();
		}
	}
	
	//get size in bytes
	size_t size = fileStats.st_size;

	//get creation time use st_ctime
	
	const time_t creationTime = fileStats.st_ctime;
	const char *cTime = ctime(&creationTime);

	//get last mod time
	
	const time_t modTime = fileStats.st_mtime;
	const char *mTime = ctime(&modTime);
	
	//build stats string
	
	sprintf(*buf,"%s\t%zu\t%s\t%s\t%s\n",type,size,cTime,mTime,fileName);
}

void createLog(char *sourceDir, char *logFilePath) { //not tested, does not deal with adding tabs

	DIR *dir = opendir(sourceDir);
	assert(dir != NULL);
	struct dirent *tempEnt; 
	FILE *newLog = fopen(logFilePath, "a");
	assert(newLog != NULL);
	char *buffer = malloc(sizeof(char)*MAXFORMATSIZE);
	assert(buffer != NULL);

	while((tempEnt = readdir(dir))){ 
		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, "..")) {
			continue;
		}
		if (S_ISDIR(tempEnt->d_type)) { 
			char *subDirSource = malloc(sizeof(sourceDir) +  	
										sizeof(tempEnt->d_name) +
										2);
			subDirSource = strcat(subDirSource, sourceDir);
			subDirSource = strcat(subDirSource, "/");
			subDirSource = strcat(subDirSource,tempEnt->d_name);
			createLog(subDirSource,logFilePath);
			free(subDirSource);
			subDirSource = NULL;
		}

		else {
			putFStats(tempEnt->d_name, &buffer); 
			fputs(buffer, newLog);
			fputc('\n',newLog);
			memset(buffer, '\0', MAXFORMATSIZE);

		}

	}

	free(buffer);
	buffer = NULL;
	fclose(newLog);
	newLog = NULL;
	closedir(dir);
	dir = NULL;
}

//Compare every char, if one differs return 1, else 0

int compareLog(FILE *oldLogFile, FILE *newLogFile) { //tested
	int c1,c2;
	while((c1 = fgetc(oldLogFile)) == (c2 = fgetc(newLogFile)) && (c1 != EOF && c2 != EOF));
	if (c1 == c2)  
		return 1;
	else
		return 0;
}

int copyFile(char *sourcePath, char *destinationPath) { //fails when given absolute path
	FILE *source = fopen(sourcePath, "r");
	assert(source != NULL);
	FILE *destination = fopen(destinationPath, "w");
	assert(destination != NULL);
	char c;
	while((c = fgetc(source)) != EOF) {
		if ((fputc(c,destination)) == EOF) {
			fprintf(stderr,"write to %s failed, function copyFile", destinationPath);
			return 0;
		}
	}
	fclose(source);
	fclose(destination);
	return 1;
}

int copyDir(char *sourceDir, char *backupDir) {
	DIR *source = opendir(sourceDir);
	assert(source != NULL);
	struct dirent *tempEnt;
	while((tempEnt = readdir(source))) {

		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, ".."))
			continue;
		if (S_ISDIR(tempEnt->d_type))
			printf("copyDir");//copyDir(this dir, folder inside backupDir);
		else 
			copyFile(sourceDir, backupDir);
	}
	return 1;
}

int getNumOfBackup(char *destinationDir) {
	DIR *destination = opendir(destinationDir);
	assert(destination != NULL);
	struct dirent *tempEnt;
	int backupCount = 0;
	while((tempEnt = readdir(destination))) {

		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, ".."))
			continue;
		if (S_ISDIR(tempEnt->d_type)) 
			backupCount++;
	}

	closedir(destination);
	
	return backupCount;
}

int removeDir(char *dirToRemove) {
	return 0;
}

int removeOldestBackup(char *destinationDir) {
	DIR *destination = opendir(destinationDir);
	assert(destination != NULL);
	struct dirent *tempEnt;
	time_t max = 0.00;
	FILE *tempFile;
	while((tempEnt = readdir(destination))) { //find oldest backup
			
		if (!strcmp(tempEnt->d_name,".") || strcmp(tempEnt->d_name,".."))
			continue;

		/*if(1) { //compare dirs time_t's, compare as floats as it is guaranteed to work
			struct stat fileStats;
			//assert(!stat(fileName, &fileStats));	
		}
		*/
	}
	//remove backup/ use recursion
	closedir(destination);
	return 0;
}
