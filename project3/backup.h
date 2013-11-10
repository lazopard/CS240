#define	DEFAULT_DEST_DIR	"./myBackup"
#define LOG_LAST_FILENAME	"log.last"
#define LOG_NEW_FILENAME	"log.new"
#define DEFAULT_MAX_BACKUPS	9

/**
 * Required function stub signatures
 */



void createLog(char *sourceDir, char *logFilePath, int level);
int compareLog(FILE *oldLogFile, FILE *newLogFile);

int copyFile(char *sourcePath, char *destinationPath);
int copyDir(char *sourceDir, char *backupDir);

int getNumOfBackup(char *destinationDir);
int removeOldestBackup(char *destinationDir);

void putCurrentTime(char **timeBuf);
