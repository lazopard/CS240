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

	/*get time use*/             /*use strftime instead of cTime*/

	const time_t modTime = fileStats.st_mtime;

	const char *mTime = ctime(&modTime);

	/*take out newLines*/

	char fcTime[strlen(cTime)];
	char fmTime[strlen(mTime)];
	strncpy(fcTime, cTime,strlen(cTime) - 1);
	fcTime[strlen(cTime) - 1] = '\0';
	strncpy(fmTime, mTime,strlen(mTime) - 1);
	fmTime[strlen(mTime) - 1] = '\0';

	/*strip filename of path*/

	if (strrchr(fileName,'/') != NULL) {
		fileName = &(strrchr(fileName,'/')[1]);
	}
	sprintf(*buf,"%s\t%zu\t%s\t%s\t%s\n",type,size,fcTime,fmTime,fileName);
}

int main(void) {
	char *buff = malloc(sizeof(char)*MAXFORMATSIZE);
	char *filename = "backup.c";
	putFStats(filename, &buff);
	printf("%s\n",buff);
	return 0;
}
