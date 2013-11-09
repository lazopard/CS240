#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int copyFile(char *sourcePath, char *destinationPath) { //fopen does not work with full path
	FILE *source = fopen(sourcePath, "r");
	assert(source != NULL);
	if (strrchr(sourcePath,'/') != NULL) {
				sourcePath = &(strrchr(sourcePath,'/')[1]);
	}
	char *fileToCopy = malloc(sizeof(char)*strlen(destinationPath) +
						sizeof(char)*strlen(sourcePath) + 2);
	sprintf(fileToCopy,"%s/%s",destinationPath,sourcePath);
	FILE *destination = fopen(fileToCopy, "w");
	if (destination == NULL) {
		perror("opening destination failed\n");
		return 0;
	}
	char c;
	while((c = fgetc(source)) != EOF) {
		if ((fputc(c,destination)) == EOF) {
			printf("write to %s failed, function copyFile", fileToCopy);
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

int compareLog(FILE *oldLogFile, FILE *newLogFile) {
	int c1,c2;
	while((c1 = fgetc(oldLogFile)) == (c2 = fgetc(newLogFile)) && (c1 != EOF && c2 != EOF));
	if (c1 == c2)  
		return 1;
	else
		return 0;
}

int copyDir(char *sourceDir, char *backupDir) { 
	DIR *source = opendir(sourceDir);
	assert(source != NULL);
	struct dirent *tempEnt;
	while((tempEnt = readdir(source))) {
		if (!strcmp(tempEnt->d_name,".") || !strcmp(tempEnt->d_name, ".."))
			continue;

		if (tempEnt->d_type == DT_DIR) {
			char *subDirSource = malloc(sizeof(sourceDir) +
					sizeof(tempEnt->d_name) +
					4);
			sprintf(subDirSource,"%s/%s",sourceDir,tempEnt->d_name);
			mkdir(subDirSource,0777);
			copyDir(subDirSource,subDirSource);
			free(subDirSource);
			subDirSource = NULL;
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

int main(void) {
	char *source = "userData";
	char *destination = "~/CS240/project3/backupDir";
	//copyDir(source,destination);
	copyFile("backup.c",destination);
}
