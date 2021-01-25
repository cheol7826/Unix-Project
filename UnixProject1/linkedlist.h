#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct list{
	char dirname[50];
	struct list* predir;
	struct list* nextdir;
}List;

List* initList();
void addList(List *head, char* dirname);
void removeList(List *head);
