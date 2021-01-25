#include "print.h"

int copyflag = 0;

// 시스템 시작 시 출력되는 함수
void printStartMenu(){
	printf("-----------------------------------------");
	printf("-----------------------------------------\n");
	printf("\t\t\t파일/디렉토리 관리 시스템\n");
	printf("-----------------------------------------");
	printf("-----------------------------------------\n");
}

// 메인 메뉴 출력함수, 현재 디렉토리 위치를 출력하고 번호 선택 가능
int printMainMenu(char * dirname){
	int choice;
	printf("-----------------------------------------");
	printf("-----------------------------------------\n");
	printf("1. 파일 / 디렉토리 상세정보\n\n");
	printf("2. 파일 실행(실행파일만 가능) / 디렉토리 이동\n\n");
	printf("3. 파일 편집 ( vi로 이동 )\n\n");
	printf("4. 파일 / 디렉토리 삭제\n\n");
	printf("5. 파일 / 디렉토리 복사\n\n");
	if(!copyflag)
		printf("\x1b[31m");
	printf("6. 파일 / 디렉토리 붙여넣기\n\n");
	printf("\x1b[0m");
	printf("7. 파일 생성\n\n");
	printf("8. 디렉토리 생성\n\n");
	printf("0. 시스템 종료\n");
	printf("-----------------------------------------");
	printf("-----------------------------------------\n");
	printf("현재 디렉토리 : %s\n", dirname);
	while(1){
		printf("-----------------------------------------");
		printf("-----------------------------------------\n");
		printf("번호 선택 >> ");
		scanf(" %d", &choice);
		while(getchar() != '\n');
	
		if(choice >= 0 && choice <= 8){
			if(!copyflag && choice == 6){
				printf("붙여넣을 파일/디렉토리가 없습니다\n");
				continue;
			}
			return choice;
		}
		printf("번호 선택 오류 : 번호를 다시 선택하세요\n");
	}
}


// 파일 번호 선택 함수
int printDirMenu(int max){	// max는 scandir에서 얻은 최대 엔트리 수
	int choice = 0;

	while(1){
		printf("파일/디렉토리 번호 선택 (취소 : 0번) >> ");
		scanf(" %d", &choice);
		while(getchar() != '\n');

		if(choice >= 0 && choice <= max)
			return choice;

		printf("번호 선택 오류 : 번호를 다시 선택하세요\n");
	}
	printf("-----------------------------------------");
	printf("-----------------------------------------\n");
	
}

