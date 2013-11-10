#define MAXDATE 9999999
#define TIMELENGTH 19
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

				char *logFilePath = malloc(sizeof(char)*strlen(destDir) +
												sizeof(char)*strlen(LOG_NEW_FILENAME) + 1);
				sprintf(logFilePath,"%s/%s",destDir,LOG_NEW_FILENAME);
				createLog(sourceDir,logFilePath,0);
				char *oldLogFilePath = malloc(sizeof(char)*strlen(destDir) +
												sizeof(char)*strlen(LOG_LAST_FILENAME) + 2);
				sprintf(oldLogFilePath,"%s/%s",destDir,LOG_LAST_FILENAME);
				FILE *newLog = fopen(logFilePath,"r");
				assert(newLog != NULL);
				FILE *oldLog = fopen(oldLogFilePath,"r");
				assert(oldLog != NULL);

				if (!compareLog(newLog, oldLog)) {
								//replace LOG_LAST_FILENAME with LOG_NEW_FILENAME
								return 1;
				}

				//replace LOG_LAST_FILENAME with LOG_NEW_FILENAME

				else {
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

//create a log with the status of all the files in sourceDir

void createLog(char *sourceDir, char *logFilePath, int level) { //formatting terribly wrong

				DIR *dir = opendir(sourceDir);
				assert(dir != NULL);
				struct dirent *tempEnt; 
				FILE *newLog = fopen(logFilePath, "a");
				assert(newLog != NULL);
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
												for(i = 0; i < level;i++) 
																fputc('\t',newLog);
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
												for(i = 0; i < level;i++) 
																fputc('\t',newLog);
												fputs(buffer, newLog);
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
								if (tempEnt->d_type == DT_DIR) {
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
												//free(subDirSource);
												//free(pathToBackup);
												subDirSource = NULL;
												pathToBackup = NULL;
								}
								else {
												char *pathToFile = malloc(sizeof(char)*strlen(tempEnt->d_name) +
																				sizeof(char)*strlen(sourceDir) + 4);
												sprintf(pathToFile,"%s/%s",sourceDir,tempEnt->d_name);

												copyFile(pathToFile,backupDir);
								}
				}
				return 1;

}

//get number of backups on destinationDir

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

//removes the oldest backup in destinationDir

int clearDir(char *pathToDir) { //not tested

				DIR *dir = opendir(pathToDir);
				assert(dir != NULL);
				struct dirent *tempEnt;

				while((tempEnt = readdir(dir))){
								printf("%s\n",tempEnt->d_name);
								if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, "..")) {
												continue;
								}
								if (tempEnt->d_type == DT_DIR) {
												char *subDirSource = malloc(sizeof(pathToDir) +
																				sizeof(tempEnt->d_name) +
																				4);
												sprintf(subDirSource,"%s/%s",pathToDir,tempEnt->d_name);
												clearDir(pathToDir);
												rmdir(pathToDir);
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
								perror("at removeOldestBackup, destination is NULL\n");
								return 0;
				}
				struct dirent *tempEnt;
				time_t min = MAXDATE;
				char *dirToRemove;
				while((tempEnt = readdir(destination))) { //find oldest backup

								if (!strcmp(tempEnt->d_name,".") || strcmp(tempEnt->d_name,"..")) {
												continue;
								}
								if (tempEnt->d_type == DT_DIR) {
												struct stat fileStats;
												char *currentDir = malloc(sizeof(char)*strlen(tempEnt->d_name) +
																				+ sizeof(char)*strlen(destinationDir) + 2);
												sprintf(currentDir,"%s/%s",tempEnt->d_name, destinationDir);
												assert(!stat(currentDir,&fileStats));	if (fileStats.st_ctime < min) {
																min = fileStats.st_ctime;
																dirToRemove = currentDir;
												}
												free(currentDir);
												currentDir = NULL;
								}
				}
				if (!clearDir(dirToRemove)) {
								perror("clearDir failed\n");
								return 0;
				}
				if (rmdir(dirToRemove)) {
								perror("rmdir failed\n");
								return 0;
				}
				closedir(destination);
				return 1;
}
