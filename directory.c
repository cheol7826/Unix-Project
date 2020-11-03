#include "directory.h"
#include "print.h"


struct dirent** list;

int dirManager(char* dirname, char* choicedir){
	int i;
	int func, dirnum;

	i = 0;

	if((i = scandir(dirname, &list, NULL, alphasort)) == -1){
		perror("ScanDir");		// 디렉토리 접근 불가능
		return -1;
	}

	printf("-----------------------------------------\n");

	for(int k=0; k<i; k++){
		printf("%3d. ", k+1);
		if(list[k]->d_type == DT_DIR)
			printf("\x1b[34m");
		printf("%s\n", list[k]->d_name);
		printf("\x1b[0m");
	}

	func = printMainMenu(dirname);
	if(func == 7){
		printf("시스템을 종료합니다.\n");
		return func;
	}

	if(func != 6){	
		dirnum = printDirMenu(i);
		if(list[dirnum-1]->d_type == DT_DIR){
			dirflag = 1;
		} else {
			dirflag = 0;
		}
		
		strcpy(choicedir, "/");
		strcat(choicedir, list[dirnum-1]->d_name);
	}

	return func;
}


