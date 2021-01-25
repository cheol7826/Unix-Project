#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#define MAX_LENGTH 256

void openfile(char *filename);
void deletefile(char *filename);
void copypastefile(char *src, char *dst);
void createfile(char *dirname);
void createdir(char *dirname);

void fileInfo(char *filename);
int dirManager(char *name, char* choicedir);
int prevflag;
