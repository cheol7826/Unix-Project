#include "print.h"

int copyflag = 0;

void printStartMenu(){
	printf("-----------------------------------------\n");
	printf("\t파일/디렉토리 관리 시스템\n");
	printf("-----------------------------------------\n");
}

int printMainMenu(char * dirname){
	int choice;
	printf("-----------------------------------------\n");
	printf("1. 파일 / 디렉토리 상세정보\n");
	printf("2. 파일 실행 / 디렉토리 이동\n");
	printf("3. 파일 편집 ( vi로 이동 )\n");
	printf("4. 파일 / 디렉토리 삭제\n");
	printf("5. 파일 / 디렉토리 복사\n");
	if(!copyflag)
		printf("\x1b[31m");
	printf("6. 파일 / 디렉토리 붙여넣기\n");
	printf("\x1b[0m");
	printf("7. 시스템 종료\n");
	printf("-----------------------------------------\n");
	printf("현재 디렉토리 : %s\n", dirname);
	while(1){
		printf("-----------------------------------------\n");
		printf("번호 선택 >> ");
		scanf(" %d", &choice);
	
		if(choice > 0 && choice <= 7){
			if(!copyflag && choice == 6){
				printf("붙여넣을 파일/디렉토리가 없습니다\n");
				continue;
			}
			return choice;
		}
		printf("번호 선택 오류 : 번호를 다시 선택하세요\n");
	}
}

int printDirMenu(int max){
	int choice;

	while(1){
		printf("파일/디렉토리 번호 선택 >> ");
		scanf(" %d", &choice);
		if(choice > 0 && choice <= max)
			return choice;

		printf("번호 선택 오류 : 번호를 다시 선택하세요\n");
	}
	printf("-----------------------------------------\n");
	
}

