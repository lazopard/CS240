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
	//build stats string
	sprintf(*buf,"%s\t%zu\t%s\t%s\t%s\n",type,size,fcTime,fmTime,fileName);
	printf("%s\n",*buf);
}

void createLog(char *sourceDir, char *logFilePath) { //fopen does not work with path

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
		if (tempEnt->d_type == DT_DIR) { 
			char *subDirSource = malloc(sizeof(sourceDir) +  	
					sizeof(tempEnt->d_name) +
					4);
			sprintf(subDirSource,"%s/%s",sourceDir,tempEnt->d_name);
			printf("%s\n",subDirSource);
			createLog(subDirSource,logFilePath);
			free(subDirSource);
			subDirSource = NULL;
		}
		else {
			printf("%s is a file\n",tempEnt->d_name);
			char *pathToFile = malloc(sizeof(char)*strlen(tempEnt->d_name) +
						  sizeof(char)*strlen(sourceDir) + 4); 
			sprintf(pathToFile,"%s/%s",sourceDir,tempEnt->d_name);
			printf("%s\n",pathToFile);
			putFStats(pathToFile, &buffer);
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

int main() {
	char *buff = malloc(sizeof(char)*100000);
	//char *filename = "userData/empty2/seriously/kidding/LICENSE";
	char *filename = "userData";
	//putFStats(filename,&buff);
	createLog(filename, "log.new");
}
