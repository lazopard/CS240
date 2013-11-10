#define MAXPATHLEN 120
#define TIMELENGTH 19
#define MAXFORMATSIZE 120
#define SOURCE "-s"
#define DESTINATION "-d"
#define MAXBACKUPS "-m"
#define NTYPES 2
#define _BSD_SOURCE

#include <signal.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "string.h"
#include "backup.h"
#include <dirent.h>
#include <assert.h>
#include <unistd.h>

int getNumOfBackup(char *destinationDir) { //tested

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

int clearDir(char *pathToDir) { //tested

	DIR *dir = opendir(pathToDir);
	if (dir == NULL) {
			perror("opendir failed\n");
			return 0;
	}

	struct dirent *tempEnt;

	while((tempEnt = readdir(dir))){
		
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


int removeOldestBackup(char *destinationDir) { //not tested
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
	printf("dirToRemove is %s\n",dirToRemove);

	if (!clearDir(dirToRemove)) {
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

int main() {
	char *destination = "backupDir";
	removeOldestBackup(destination);
}
