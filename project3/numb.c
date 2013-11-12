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

int getNumOfBackup(char *destinationDir) { /* I make the assumption that every dir in
																							destinationDir is a backup */
				DIR *destination = opendir(destinationDir);
				assert(destination != NULL);
				struct dirent *tempEnt;
				int backupCount = 0;
				while((tempEnt = readdir(destination))) {
								struct stat tempStat;
								stat(tempEnt->d_name, &tempStat);
								if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, "..")) {
												continue;
								}
								if(S_ISDIR(tempStat.st_mode)) {
												backupCount++;
								}
				}

				closedir(destination);

				return backupCount;
}

int main(void) {
				char *destination = "myBackup";
				int numOfBackups = getNumOfBackup(destination);
				printf("%d\n",numOfBackups);
				return 1;
}
