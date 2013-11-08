#include <assert.h>
#include <stdio.h>
#include <stdlib.h> 

int copyFile(char *sourcePath, char *destinationPath) {
	FILE *source = fopen(sourcePath, "r");
	assert(source != NULL);
	FILE *destination = fopen(destinationPath, "w");
	assert(destination != NULL);
	char c;
	while((c = fgetc(source)) != EOF) {
		if ((fputc(c,destination)) == EOF) {
			printf("write to %s failed, function copyFile", destinationPath);
			return 0;
		}
	}
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
			copyDir(subDirSource,backupDir);
			free(subDirSource);
			subDirSource = NULL;
		}
		else {
			char *pathToFile = malloc(sizeof(char)*strlen(tempEnt->d_name) +
					sizeof(char)*strlen(sourceDir) + 4);
			sprintf(pathToFile,"%s/%s",sourceDir,tempEnt->d_name);
			copyFile(pathToFile,backupDir)
		}


	}
	return 1;
}

int main(void) {
	char *source = "backup.c";
	char *destination = "newBackup.c";
	(compareLog(fopen(source, "r"), fopen(destination, "r"))) ? printf("copyFile succeded\n") : printf("copyFile failed\n");
	return 0;
}
