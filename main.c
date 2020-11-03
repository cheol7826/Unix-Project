#include "directory.h"
#include "print.h"
#include "linkedlist.h"
#include <unistd.h>
#include <pwd.h>

void makedirname(List *head, char *dirname);

int main(void){
	char dirname[256];
	char targetdir[256];
	char sourcedir[256];
	char command[256];
	struct passwd * pw;
	uid_t id;
	int choice;
	List *pointer;
	extern int dirflag;
	extern int copyflag;



	id = getuid();
	pw = getpwuid(id);
	
	pointer = initList(pw->pw_dir);

	printStartMenu();

	strcpy(targetdir, pw->pw_dir);

	while(1){
		if(dirflag != 1){
			removeList(pointer);
			makedirname(pointer, targetdir);
		}	
		choice = dirManager(targetdir, dirname);
		
		if(choice == 7)
			break;
			

		if(strcmp(dirname, "/..") == 0){
			if(strcmp(targetdir, pw->pw_dir) == 0){
				printf("더 이상 이동할 수 없습니다\n");
				continue;
			}
			removeList(pointer);
		}
		else if(strcmp(dirname, "/.") == 0)
			continue;
		else
			addList(pointer, dirname);

		makedirname(pointer, targetdir);

		char yorn;

		switch(choice){
			case 1:
				break;
			case 2:
				if(dirflag == 0){
					printf("%s 파일 실행\n", targetdir);
					system(targetdir);
					system("sleep 1");
				}
				break;
			case 3:
				if(dirflag == 0){
					strcpy(command, "vi ");
					strcat(command, targetdir);
					system(command);
				} else{
					printf("디렉토리는 실행할 수 없습니다\n");
				}
				break;
			case 4:
				printf("정말 %s 을 삭제하시겠습니까? (y | n)\n", targetdir);
				scanf(" %c", &yorn);
				if(yorn == 'y'){
					strcpy(command, "rm -rf ");
					strcat(command, targetdir);
					system(command);
				}

				removeList(pointer);
				makedirname(pointer, targetdir);
				break;
			case 5:
				strcpy(sourcedir, targetdir);
				printf("%s 파일을 복사했습니다 \n", sourcedir);
				copyflag = 1;
				break;

			case 6:
				removeList(pointer);
				makedirname(pointer, targetdir);
				strcpy(command, "cp ");
				strcat(command, sourcedir);
				strcat(command, " ");
				strcat(command, targetdir);
				printf("%s\n", command);
				system(command);
				copyflag = 0;
				break;


		}
				
	}

	
	return 0;

}


void makedirname(List *head, char *dirname){
	strcpy(dirname, head->dirname);
	List *target = head->nextdir;
	while(target != head){
		strcat(dirname, target->dirname);
		target = target->nextdir;
	}
}


