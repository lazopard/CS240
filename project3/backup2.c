#define MAXPATHLEN 200
#define MAXDATE 9999999
#define TIMELENGTH 80
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

void putCurrentTime(char **timeBuf);

void putFStats(char *fileName, char **buf);

void createLogFile(char *sourceDir, char *logFilePath, int level);

int compareLog(FILE *oldLogFile, FILE *newLogFile);

int copyFile(char *sourcePath, char *destinationPath);

int copyDir(char *sourceDir, char *backupDir);

int getNumOfBackup(char *destinationDir);

int clearDir(char *pathToDir);

int removeOldestBackup(char *destinationDir);

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
			mkdir(destDir, 0777);
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
		mkdir(DEFAULT_DEST_DIR, 0777);
	}
	if (!foundMaxB) {
		maxB = DEFAULT_MAX_BACKUPS;
	}

	/* Arguments processing end */

	char *newLogFilePath = malloc(sizeof(char)*strlen(destDir) + /*create new log*/
			sizeof(char)*strlen(LOG_NEW_FILENAME) + 2);
	if (destDir[strlen(destDir) - 1] != '/') { /*add slash if neccesary*/
		sprintf(newLogFilePath,"%s/%s",destDir,LOG_NEW_FILENAME);
	}
	else {
		sprintf(newLogFilePath,"%s%s",destDir,LOG_NEW_FILENAME);
	}

	createLogFile(sourceDir,newLogFilePath,0);

	char *oldLogFilePath = malloc(sizeof(char)*strlen(destDir) +
			sizeof(char)*strlen(LOG_LAST_FILENAME) + 2);
	if (destDir[strlen(destDir) - 1] != '/') {
		sprintf(oldLogFilePath,"%s/%s",destDir,LOG_LAST_FILENAME);
	}
	else {
		sprintf(oldLogFilePath,"%s%s",destDir,LOG_LAST_FILENAME);
	}

	FILE *newLog = fopen(newLogFilePath,"r");

	if (newLog == NULL) {
		perror("fopen failed");
		printf(" trying to open %s\n",newLogFilePath);
		return 0;
	}

	FILE *oldLog = fopen(oldLogFilePath,"r");

	/*rewind(newLog);
	  rewind(oldLog);
	  */

	if (oldLog == NULL) { /*if it is the first time backing up*/
		fclose(newLog);
		rename(newLogFilePath, oldLogFilePath);
		free(newLogFilePath);
		newLogFilePath = NULL;
		char *backupPath = malloc(sizeof(char)*strlen(destDir) + 
				sizeof(char)*TIMELENGTH + 1);
		char *currentTime = malloc(sizeof(char)*TIMELENGTH);
		putCurrentTime(&currentTime);
		sprintf(backupPath,"%s/%s",destDir,currentTime);
		mkdir(backupPath,0777);
		if (!copyDir(sourceDir,backupPath)) {
			perror("Copying the directory failed.\n");
			return 0;
		}
		free(backupPath);
		free(oldLogFilePath);
		backupPath = NULL;
		free(currentTime);
		currentTime = NULL;
	}

	else if (compareLog(newLog, oldLog)) { /*if they are the same, just rename log.new*/
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

	else { /*if there is any change, create a new backup*/
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

	/*check number of backups, removeOldest if neccesary*/
	int numBackups = getNumOfBackup(destDir);
	printf("num of backups is %d, maxB is %d\n", numBackups, maxB);
	if (numBackups > maxB) {

		if(!removeOldestBackup(destDir)) {
			perror("Removing oldest backup failed.\n");
			return 0;
		}
	}

	return 1;
}

/*store current time with the appropiate format in timeBuff*/

void putCurrentTime(char **timeBuf) {

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	sprintf(*timeBuf, "%4d-%02d-%02d-%02d-%02d-%02d", timeinfo->tm_year + 1900,
			timeinfo->tm_mon + 1,
			timeinfo->tm_mday, timeinfo->tm_hour,
			timeinfo->tm_min, timeinfo->tm_sec);
}

/*put a formatted string with the stats of a file in buff*/

void putFStats(char *fileName, char **buf) {

	struct stat fileStats;
	assert(!stat(fileName, &fileStats));
	int i;

	/*get type*/
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

	/*get size in bytes*/

	size_t size = fileStats.st_size;

	/*get last mod time*/

	char *mTime = malloc(sizeof(char)*TIMELENGTH + 1);
	strftime(mTime,TIMELENGTH , "%c",localtime(&fileStats.st_mtime));

	/*take out newLines*/

	char fmTime[strlen(mTime)];

	strncpy(fmTime, mTime,strlen(mTime) - 1);
	fmTime[strlen(mTime) - 1] = '\0';

	/*strip filename of path*/

	if (strrchr(fileName,'/') != NULL) {
		fileName = &(strrchr(fileName,'/')[1]);
	}

	/*build stats string*/

	sprintf(*buf,"%s\t%zu\t%s\t%s\t%s\n",type,size,fmTime,fmTime,fileName);

	free(mTime);
	mTime = NULL;
}

/*filter function for scandir*/

int skipWdPd(const struct dirent *dir) {
	return !(!strcmp(dir->d_name,".") || !strcmp(dir->d_name, ".."));
}


/*create a log with the status of all the files in sourceDir*/

void createLogFile(char *sourceDir, char *logFilePath, int level) {

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

	n = scandir(sourceDir, &dirList, skipWdPd, alphasort); /*filter working and parent dirs*/

	if (n < 0) {
		perror("scandir failed");
	}

	else {
		while (n--) {   

			char *fullEntPath = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
					sizeof(char)*strlen(sourceDir) + 2);
			if (dirList[n]->d_name[strlen(dirList[n]->d_name) - 1] != '/') {
				sprintf(fullEntPath,"%s/%s",sourceDir,dirList[n]->d_name);
			}

			else {
				sprintf(fullEntPath,"%s%s",sourceDir,dirList[n]->d_name);
			}

			struct stat *tempStat = malloc(sizeof(struct stat));

			if(stat(fullEntPath, tempStat)) {
				perror("stat() failed\n");
				abort();
			}		

			if (S_ISDIR(tempStat->st_mode)) { 

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
				createLogFile(subDirSource,logFilePath,level + 1);
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
				for(i = 0; i < level;i++) { /*add right amount of tabs*/
					fputc('\t',newLog);
				}
				fputs(buffer, newLog);
				memset(buffer, '\0', MAXFORMATSIZE);
				free(pathToFile);
				free(dirList[n]);
				pathToFile = NULL;
			}
			free(fullEntPath);
			free(tempStat);
			tempStat = NULL;
		}
		free(dirList);
		dirList = NULL;
	}
	free(buffer);
	buffer = NULL;
	fclose(newLog);
	newLog = NULL;

}


/*Compare every char, if one differs return 1, else 0*/

int compareLog(FILE *oldLogFile, FILE *newLogFile) {
	if (oldLogFile == NULL || newLogFile == NULL) {
		return 0;
	}
	int c1,c2;
	while((c1 = fgetc(oldLogFile)) == (c2 = fgetc(newLogFile)) && (c1 != EOF && c2 != EOF));
	if (c1 == c2) {
		return 1;
	}
	else {
		return 0;
	}
}

/*copies a file*/

int copyFile(char *sourcePath, char *destinationPath) {

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

	/*copy a byte at a time*/
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

/*copies a directory*/

int copyDir(char *sourceDir, char *backupDir) {

	struct dirent **dirList;

	int n = scandir(sourceDir, &dirList, skipWdPd, alphasort);

	if (n < 0) {
		perror("scandir failed");
		return 0;
	}

	while(n--) {

		char *fullEntPath = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
				sizeof(char)*strlen(sourceDir) + 2);
		if (dirList[n]->d_name[strlen(dirList[n]->d_name) - 1] != '/') {
			sprintf(fullEntPath,"%s/%s",sourceDir,dirList[n]->d_name);
		}

		else {
			sprintf(fullEntPath,"%s%s",sourceDir,dirList[n]->d_name);
		}


		struct stat *tempStats = malloc(sizeof(struct stat));
		if (stat(fullEntPath, tempStats)) {
			perror("stat() failed, function copyDir\n");
			printf("trying to open %s\n", dirList[n]->d_name);
			return 0;
		}

		if (S_ISDIR(tempStats->st_mode)) { /* if dir mkdir in backupDir, and copyDir on it*/ 

			char *subDirSource = malloc(sizeof(sourceDir) +
					sizeof(dirList[n]->d_name) +
					2);
			sprintf(subDirSource,"%s/%s",sourceDir,dirList[n]->d_name);
			sprintf(subDirSource,"%s%s",sourceDir,dirList[n]->d_name);
			if (strrchr(subDirSource,'/') != NULL) {
				subDirSource = &(strrchr(subDirSource,'/')[1]);
			}

			char *pathToBackup = malloc(sizeof(char)*strlen(backupDir) +
					sizeof(char)*strlen(subDirSource) +
					2);
			sprintf(pathToBackup,"%s/%s",backupDir,subDirSource);
			mkdir(pathToBackup,0777);
			sprintf(subDirSource,"%s/%s",sourceDir,dirList[n]->d_name);
			copyDir(subDirSource,pathToBackup);
			subDirSource = NULL;
			pathToBackup = NULL;
		}

		else { /*if file, copy*/
			char *pathToFile = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
					sizeof(char)*strlen(sourceDir) + 4);
			sprintf(pathToFile,"%s/%s",sourceDir,dirList[n]->d_name);
			copyFile(pathToFile,backupDir);
			free(pathToFile);
			pathToFile = NULL;
		}
		free(tempStats);
		tempStats = NULL;
		free(dirList[n]);
		free(fullEntPath);
		fullEntPath = NULL;
	}
	free(dirList);
	dirList = NULL;

	return 1;
}

/*get number of backups on destinationDir*/

int getNumOfBackup(char *destinationDir) { /* I make the assumption that every dir in
					      destinationDir is a backup */
	struct dirent **dirList;
	int backupCount, n;
	backupCount = 0;
	n = scandir(destinationDir, &dirList, skipWdPd, alphasort);
	if (n < 0) {
		perror("scandir failed function getNumOfBackup\n");
		return -1;
	}
	else {
		while(n--) {
			char *fullEntPath = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
					sizeof(char)*strlen(destinationDir) + 2);
			if (dirList[n]->d_name[strlen(dirList[n]->d_name) - 1] != '/') {
				sprintf(fullEntPath,"%s/%s",destinationDir,dirList[n]->d_name);
			}

			else {
				sprintf(fullEntPath,"%s%s",destinationDir,dirList[n]->d_name);
			}

			struct stat *tempStat = malloc(sizeof(struct stat));

			if(stat(fullEntPath, tempStat)) {
				perror("stat failed, function getNumBackup");
				return -1;
			}

			if(S_ISDIR(tempStat->st_mode)) {
				++backupCount;
			}
			free(dirList[n]);
			free(fullEntPath);
			free(tempStat);
			tempStat = NULL;
		}
		free(dirList);
		dirList = NULL;
	}

	return backupCount;
}

/*removes the oldest backup in destinationDir*/

int clearDir(char *pathToDir) {

	struct dirent **dirList;
	int n = scandir(pathToDir, &dirList, skipWdPd, alphasort);

	if (n < 0) {
		perror("scandir failed, function clearDir");
		return 0;
	}

	while(n--) { /*if file remove, if dir clearDir and remove*/

		char *fullEntPath = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
				sizeof(char)*strlen(pathToDir) + 2);
		if (dirList[n]->d_name[strlen(dirList[n]->d_name) - 1] != '/') {
			sprintf(fullEntPath,"%s/%s",pathToDir,dirList[n]->d_name);
		}

		else {
			sprintf(fullEntPath,"%s%s",pathToDir,dirList[n]->d_name);
		}


		struct stat *tempStat = malloc(sizeof(struct stat));
		if (stat(fullEntPath, tempStat)) {
			perror("stat failed function clearDir\n");
			return 0;
		}

		if (S_ISDIR(tempStat->st_mode)) {
			char *subDirSource = malloc(sizeof(pathToDir) +
					sizeof(dirList[n]->d_name) +
					4);
			sprintf(subDirSource,"%s/%s",pathToDir,dirList[n]->d_name);
			clearDir(subDirSource);
			rmdir(subDirSource);
			free(subDirSource);
			subDirSource = NULL;
		}

		else {
			char *pathToFile = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
					sizeof(char)*strlen(pathToDir) + 4);
			sprintf(pathToFile,"%s/%s",pathToDir,dirList[n]->d_name);
			remove(pathToFile);
			free(pathToFile);
			pathToFile = NULL;
		}
		free(dirList[n]);
		free(fullEntPath);
		free(tempStat);
		fullEntPath = NULL;
		tempStat = NULL;

	}
	free(dirList);
	dirList = NULL;
	return 1;
}

int removeOldestBackup(char *destinationDir) {

	struct dirent **dirList;
	time_t min = LONG_MAX;
	char *dirToRemove = malloc(sizeof(char)*MAXPATHLEN);

	int n = scandir(destinationDir, &dirList, skipWdPd, alphasort);

	if (n < 0) {
		perror("scandir failed, function removeOldestBackup");
		return 0;
	}

	while(n--) { /*find oldest backup*/

		char *fullEntPath = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
				sizeof(char)*strlen(destinationDir) + 2);
		if (dirList[n]->d_name[strlen(dirList[n]->d_name) - 1] != '/') {
			sprintf(fullEntPath,"%s/%s",destinationDir,dirList[n]->d_name);
		}

		else {
			sprintf(fullEntPath,"%s%s",destinationDir,dirList[n]->d_name);
		}


		struct stat *tempStat = malloc(sizeof(struct stat));
		if (stat(fullEntPath, tempStat)) {
			perror("stat failed, function removeOldestBackup\n");
			return 0;
		}

		if (S_ISDIR(tempStat->st_mode)) {
			struct stat fileStats;
			char *currentDir = malloc(sizeof(char)*strlen(dirList[n]->d_name) +
					+ sizeof(char)*strlen(destinationDir) + 2);
			sprintf(currentDir,"%s/%s",destinationDir, dirList[n]->d_name);
			if (stat(currentDir,&fileStats)) {
				perror("calling stat on currentDir failed");
				printf("currentDir is %s\n",currentDir);
				return 0;
			}
			if (fileStats.st_mtime < min) {
				min = fileStats.st_mtime;
				dirToRemove = strcpy(dirToRemove,currentDir);
			}
			free(currentDir);
			currentDir = NULL;
		}
		free(dirList[n]);
		free(fullEntPath);
		fullEntPath = NULL;
		free(tempStat);
		tempStat = NULL;
	}
	free(dirList);
	dirList = NULL;

	if (!clearDir(dirToRemove)) { /*remove every file inside the directory*/
		perror("clearDir failed\n");
		return 0;
	}
	if (rmdir(dirToRemove)) {
		perror("rmdir failed\n");
		return 0;
	}

	free(dirToRemove);
	return 1;
}

