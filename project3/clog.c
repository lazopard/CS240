#define MAXFORMATSIZE 120
#define SOURCE "-s"
#define DESTINATION "-d"
#define MAXBACKUPS "-m"
#define NTYPES 2
#define _BSD_SOURCE

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "string.h"
#include "backup.h"
#include <dirent.h>
#include <assert.h> 
void putFStats(char *fileName, char **buf) { //tested

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

void createLog(char *sourceDir, char *logFilePath, int level) {

	DIR *dir = opendir(sourceDir);
	assert(dir != NULL);
	struct dirent *tempEnt; 
	FILE *newLog = fopen(logFilePath, "a");
	if (newLog == NULL) {
		perror("fopen failed opening logFilePath, function newLog");
		return;
	}
	char *buffer = malloc(sizeof(char)*MAXFORMATSIZE);
	assert(buffer != NULL);
	int i;

	while((tempEnt = readdir(dir))){ 
		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, "..")) {
			continue;
		}
		if (tempEnt->d_type == DT_DIR) { 
			char *subDirSource = malloc(sizeof(sourceDir) +  	
					sizeof(tempEnt->d_name) +
					4);
			sprintf(subDirSource,"%s/%s",sourceDir,tempEnt->d_name);
			putFStats(subDirSource, &buffer);
			for(i = 0; i < level;i++) {
				fputc('\t',newLog);
			}
			fputs(buffer, newLog);
			memset(buffer, '\0', MAXFORMATSIZE);
			createLog(subDirSource,logFilePath,level + 1);
			free(subDirSource);
			subDirSource = NULL;
		}
		else {
			char *pathToFile = malloc(sizeof(char)*strlen(tempEnt->d_name) +
					sizeof(char)*strlen(sourceDir) + 4); 
			sprintf(pathToFile,"%s/%s",sourceDir,tempEnt->d_name);
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
	free(buffer);
	buffer = NULL;
	fclose(newLog);
	newLog = NULL;
	closedir(dir);
	dir = NULL;
}

int skipWdPd(const struct dirent *dir) {
	 return !(!strcmp(dir->d_name,".") || !strcmp(dir->d_name, ".."));
}

void createLog2(char *sourceDir, char *logFilePath, int level) {

	struct dirent **dirList;
	FILE *newLog = fopen(logFilePath, "a");
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

			if (dirList[n]->d_type == DT_DIR) { 
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
				createLog(subDirSource,logFilePath,level + 1);
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

int main() {
	char *buff = malloc(sizeof(char)*100000);
	char *filename = "userData";
	createLog2(filename, "log.new",0);
	free(buff);
}
