#define MAXPATHLEN 200
#define MAXDATE 9999999
#define TIMELENGTH 19
#define MAXFORMATSIZE 120
#define SOURCE "-s"
#define DESTINATION "-d"
#define MAXBACKUPS "-m"
#define NTYPES 2
#define _BSD_SOURCE

#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "string.h"
#include "backup.h"
#include <dirent.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char **argv) {

	/* Arguments processing */

	char *sourceDir, *destDir;
	int foundS, foundDest, foundMaxB, i, maxB;

	foundS=maxB=foundDest=foundMaxB=0;

	for(i=1; i < argc;i++) {
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

	char *newLogFilePath = malloc(sizeof(char)*strlen(destDir) + //create new log
			sizeof(char)*strlen(LOG_NEW_FILENAME) + 1);
	sprintf(newLogFilePath,"%s/%s",destDir,LOG_NEW_FILENAME);
	createLog(sourceDir,newLogFilePath,0);
	char *oldLogFilePath = malloc(sizeof(char)*strlen(destDir) +
			sizeof(char)*strlen(LOG_LAST_FILENAME) + 2);
	sprintf(oldLogFilePath,"%s/%s",destDir,LOG_LAST_FILENAME);
	FILE *newLog = fopen(newLogFilePath,"r");

	if (newLog == NULL) {
		perror("fopen failed");
		printf(" trying to open %s\n",newLogFilePath);
		return 0;
	}
	FILE *oldLog = fopen(oldLogFilePath,"r");
	if (oldLog == NULL) {
		perror("fopen failed");
		printf(" trying to open %s\n",oldLogFilePath);
		return 0;
	}

	if (!compareLog(newLog, oldLog)) { //if they are the same, just rename log.new
		fclose(oldLog);
		fclose(newLog);
		remove(oldLogFilePath);
		rename(newLogFilePath, oldLogFilePath);
		free(newLogFilePath);
		free(oldLogFilePath);
		newLogFilePath = NULL;
		oldLogFilePath = NULL;
		return 1;
	}

	else { //if there is any change, create a new backup
		fclose(oldLog);
		fclose(newLog);
		remove(oldLogFilePath);
		rename(newLogFilePath, oldLogFilePath);
		free(newLogFilePath);
		free(oldLogFilePath);
		newLogFilePath = NULL;
		oldLogFilePath = NULL;
		char *backupPath = malloc(sizeof(char)*strlen(destDir) + 
				sizeof(char)*TIMELENGTH);
		char *currentTime = malloc(sizeof(char)*TIMELENGTH);
		putCurrentTime(&currentTime);
		sprintf(backupPath,"%s/%s",destDir,currentTime);
		mkdir(backupPath,0777);
		if (!copyDir(sourceDir,backupPath)) {
			perror("Copying the directory failed.\n");
			return 0;
		}
		free(backupPath);
		backupPath = NULL;
		free(currentTime);
		currentTime = NULL;
	}

	//check number of backups, removeOldest if neccesary
	if (getNumOfBackup(destDir) > maxB) {
		if(!removeOldestBackup(destDir)) {
			perror("Removing oldest backup failed.\n");
			return 0;
		}
	}

	return 1;
}

//store current time with the appropiate format in timeBuff

void putCurrentTime(char **timeBuf) { //tested

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	sprintf(*timeBuf, "%d-%d-%d-%d-%d-%d", timeinfo->tm_year + 1900,
			timeinfo->tm_mon + 1,
			timeinfo->tm_mday, timeinfo->tm_hour,
			timeinfo->tm_min, timeinfo->tm_sec);
}



//put a formatted string with the stats of a file in buff

void putFStats(char *fileName, char **buf) { //tested

	struct stat fileStats;
	assert(!stat(fileName, &fileStats));
	int i;

	//get type 
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
	size_t size = fileStats.st_size;

	//get creation time use st_ctime

	const time_t creationTime = fileStats.st_ctime;
	const char *cTime = ctime(&creationTime);

	//get last mod time

	const time_t modTime = fileStats.st_mtime;
	const char *mTime = ctime(&modTime);

	//take out newLines
	char fcTime[strlen(cTime)];
	char fmTime[strlen(mTime)];
	strncpy(fcTime, cTime,strlen(cTime) - 1);
	fcTime[strlen(cTime) - 1] = '\0';
	strncpy(fmTime, mTime,strlen(mTime) - 1);
	fmTime[strlen(mTime) - 1] = '\0';
	//strip filename of path
	if (strrchr(fileName,'/') != NULL) {
		fileName = &(strrchr(fileName,'/')[1]);
	}
	//build stats string
	sprintf(*buf,"%s\t%zu\t%s\t%s\t%s\n",type,size,fcTime,fmTime,fileName);
}

//filter function for scandir

int skipWdPd(const struct dirent *dir) {
	return !(!strcmp(dir->d_name,".") || !strcmp(dir->d_name, ".."));
}


//create a log with the status of all the files in sourceDir

void createLog(char *sourceDir, char *logFilePath, int level) { //tested

	struct dirent **dirList;
	FILE *newLog;

	if (level == 0) {
		newLog = fopen(logFilePath,"w+");   
	}
	else {
		newLog = fopen(logFilePath, "a");
	}

	char *buffer = malloc(sizeof(char)*MAXFORMATSIZE);
	assert(buffer != NULL);

	if (newLog == NULL) {
		perror("fopen failed opening logFilePath, function newLog");
		return;
	}

	int n, i;

	n = scandir(sourceDir, &dirList, skipWdPd, alphasort); //filter working and parent dirs

	if (n < 0)
		perror("scandir failed");
	else {
		while (n--) {   
			if (((int)dirList[n]->d_type) == ((int) DT_DIR)) { 
				char *subDirSource = malloc(sizeof(sourceDir) +     
						sizeof(dirList[n]->d_name) +
						4);
				sprintf(subDirSource,"%s/%s",sourceDir,dirList[n]->d_name);
				putFStats(subDirSource, &buffer);
				for(i = 0; i < level;i++) {
					fputc('\t',newLog);
				}
				fputs(buffer, newLog);
				memset(buffer, '\0', MAXFORMATSIZE);
				fclose(newLog);
				createLog(subDirSource,logFilePath,level + 1);
				newLog = fopen(logFilePath, "a");
				free(subDirSource);
				free(dirList[n]);
				subDirSource = NULL;
			}

			else {
				char *pathToFile = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
						sizeof(char)*strlen(sourceDir) + 4); 
				sprintf(pathToFile,"%s/%s",sourceDir,dirList[n]->d_name);
				putFStats(pathToFile, &buffer);
				for(i = 0; i < level;i++) { //add right amount of tabs
					fputc('\t',newLog);
				}
				fputs(buffer, newLog);
				memset(buffer, '\0', MAXFORMATSIZE);
				free(pathToFile);
				pathToFile = NULL;
			}
		}
		free(dirList);
	}
	free(buffer);
	buffer = NULL;
	fclose(newLog);
	newLog = NULL;

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

//copies a file

int copyFile(char *sourcePath, char *destinationPath) { //tested

	FILE *source = fopen(sourcePath, "r");
	if (source == NULL) {
		perror("opening source failed\n");
		return 0;
	}
	if (strrchr(sourcePath,'/') != NULL) {
		sourcePath = &(strrchr(sourcePath,'/')[1]);
	}

	char *fileToCopy = malloc(sizeof(char)*strlen(destinationPath) +
			sizeof(char)*strlen(sourcePath) + 2);
	sprintf(fileToCopy,"%s/%s",destinationPath,sourcePath);
	FILE *destination = fopen(fileToCopy, "w");
	if (destination == NULL) {
		perror("opening failed\n");
		printf("destination is %s\n", fileToCopy);
		return 0;
	}

	//copy a byte at a time
	char c;
	while((c = fgetc(source)) != EOF) {
		if ((fputc(c,destination)) == EOF) {
			perror("write to file failed, function copyFile");
			free(fileToCopy);
			fclose(source);
			fclose(destination);
			return 0;
		}
	}

	free(fileToCopy);
	fclose(source);
	fclose(destination);
	return 1;
}

//copies a directory

int copyDir(char *sourceDir, char *backupDir) { //tested, does not free memory

	DIR *source = opendir(sourceDir);
	if (source == NULL) {
		perror("opendir failed, function copyDir");
		return 0;
	}
	struct dirent *tempEnt;

	while((tempEnt = readdir(source))) {
		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, ".."))
			continue;

		if (tempEnt->d_type == DT_DIR) { // if dir mkdir in backupDir, and copyDir on it 
			char *subDirSource = malloc(sizeof(sourceDir) +
					sizeof(tempEnt->d_name) +
					4);
			sprintf(subDirSource,"%s/%s",sourceDir,tempEnt->d_name);
			if (strrchr(subDirSource,'/') != NULL) {
				subDirSource = &(strrchr(subDirSource,'/')[1]);
			}
			char *pathToBackup = malloc(sizeof(char)*strlen(backupDir) +
					sizeof(char)*strlen(subDirSource) +
					2);
			sprintf(pathToBackup,"%s/%s",backupDir,subDirSource);
			mkdir(pathToBackup,0777);
			sprintf(subDirSource,"%s/%s",sourceDir,tempEnt->d_name);
			copyDir(subDirSource,pathToBackup);
			free(pathToBackup);
			free(subDirSource);
			subDirSource = NULL;
			pathToBackup = NULL;
		}

		else { //if file, copy
			char *pathToFile = malloc(sizeof(char)*strlen(tempEnt->d_name) +
					sizeof(char)*strlen(sourceDir) + 4);
			sprintf(pathToFile,"%s/%s",sourceDir,tempEnt->d_name);
			copyFile(pathToFile,backupDir);
			free(pathToFile);
			pathToFile = NULL;
		}
	}

	return 1;
}

//get number of backups on destinationDir

int getNumOfBackup(char *destinationDir) { // I make the assumption that every dir in 
	// destinationDir is a backup

	DIR *destination = opendir(destinationDir);
	assert(destination != NULL);
	struct dirent *tempEnt; int backupCount = 0;
	while((tempEnt = readdir(destination))) {
		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, ".."))
			continue;
		if(tempEnt->d_type == DT_DIR) 
			backupCount++;
	}

	closedir(destination);

	return backupCount;
}

//removes the oldest backup in destinationDir

int clearDir(char *pathToDir) { //tested

	DIR *dir = opendir(pathToDir);
	if (dir == NULL) {
		perror("opendir failed\n");
		return 0;
	}

	struct dirent *tempEnt;

	while((tempEnt = readdir(dir))){ //if file remove, if dir clearDir and remove

		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, "..")) {
			continue;
		}

		if (tempEnt->d_type == DT_DIR) {
			char *subDirSource = malloc(sizeof(pathToDir) +
					sizeof(tempEnt->d_name) +
					4);
			sprintf(subDirSource,"%s/%s",pathToDir,tempEnt->d_name);
			clearDir(subDirSource);
			rmdir(subDirSource);
			free(subDirSource);
			subDirSource = NULL;
		}

		else {
			char *pathToFile = malloc(sizeof(char)*strlen(tempEnt->d_name) +
					sizeof(char)*strlen(pathToDir) + 4);
			sprintf(pathToFile,"%s/%s",pathToDir,tempEnt->d_name);
			remove(pathToFile);
			free(pathToFile);
			pathToFile = NULL;
		}

	}
	closedir(dir);
	dir = NULL;
	return 1;
}

int removeOldestBackup(char *destinationDir) { //tested

	DIR *destination = opendir(destinationDir);
	if (destination == NULL) {
		perror("opendir at removeOldestBackup failed\n");
		return 0;
	}
	struct dirent *tempEnt;
	time_t min = LONG_MAX;
	char *dirToRemove = malloc(sizeof(char)*MAXPATHLEN);

	while((tempEnt = readdir(destination))) { //find oldest backup
		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name,"..")) {
			continue;
		}
		else if (tempEnt->d_type == DT_DIR) {
			struct stat fileStats;
			char *currentDir = malloc(sizeof(char)*strlen(tempEnt->d_name) +
					+ sizeof(char)*strlen(destinationDir) + 2);
			sprintf(currentDir,"%s/%s",destinationDir, tempEnt->d_name);
			if (stat(currentDir,&fileStats)) {
				perror("calling stat on currentDir failed");
				printf("currentDir is %s\n",currentDir);
				return 0;
			}
			if (fileStats.st_ctime < min) {
				min = fileStats.st_ctime;
				dirToRemove = strcpy(dirToRemove,currentDir);
			}
			free(currentDir);
			currentDir = NULL;
		}
	}

	if (!clearDir(dirToRemove)) { //remove every file inside the directory
		perror("clearDir failed\n");
		return 0;
	}
	if (rmdir(dirToRemove)) {
		perror("rmdir failed\n");
		return 0;
	}

	free(dirToRemove);
	closedir(destination);
	return 1;
}
