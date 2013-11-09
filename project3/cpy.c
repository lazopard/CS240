#define _BSD_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int copyFile(char *sourcePath, char *destinationPath) { //fopen does not work with full path
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

int main(void) {
	char *source = "userData";
	char *destination = "/home/adminuser/CS/CS240/project3/backupDir";
	copyDir(source,destination);
	//copyFile("backup.c",destination);
}
