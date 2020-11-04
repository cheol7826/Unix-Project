#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


void fileInfo(char *filename);
int dirManager(char *name, char* choicedir);
int dirflag;
