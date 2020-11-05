/* 파일, 디렉토리 관리 시스템 */

#include "directory.h"				
#include "print.h"
#include "linkedlist.h"
#include <unistd.h>
#include <pwd.h>
#define MAX_LENGTH 256

/* directory.h  : 파일, 디렉토리 정보 및 디렉토리 엔트리를 불러오는 함수가 존재하는 헤더파일
 * print.h      : 시스템에서의 화면 출력 부분, 각종 출력 형식들이 저장된 헤더파일
 * linkedlist.h : 이중 연결 리스트 관련 헤더파일
 * 		-> 연결리스트는 사용자의 홈 디렉토리를 기준으로 하여 하위 디렉토리에 대한 이름들을
 * 		   리스트를 통해서 관리하는 형태
 */


void makedirname(List *head, char *dirname);
void openfile(char *filename);
void deletefile(char *filename);
void copypastefile(char *src, char *dst);
void createfile(char *dirname);
void createdir(char *dirname);

extern int prevflag;
extern int copyflag;

int main(void){
	char dirname[MAX_LENGTH];		// 선태한 파일이나 디렉토리 이름을 얻는 공간
	char targetdir[MAX_LENGTH];		// 현재 선택한 디렉토리나 파일의 절대 경로를 얻는 공간
	char sourcedir[MAX_LENGTH];		// 복사, 붙여넣기에 사용되는 파일이름 저장 공간
	struct passwd * pw;
	uid_t id;
	int choice;
	List *pointer;				// 연결리스트 head부분

	// 현재 사용자의 정보를 passwd 구조체에 얻어오는 부분
	id = getuid();
	pw = getpwuid(id);
	
	// 현재 사용자의 홈 디렉토리를 기준으로 시스템 시작
	pointer = initList(pw->pw_dir);
	strcpy(targetdir, pw->pw_dir);	

	// print.h에 존재하는 함수로 시스템 시작 출력 부분
	printStartMenu();

	// 시스템 종료를 누르기 전까지는 while문에서 계속 반복
	while(1){
		// prevflag는 targetdir의 절대경로에서 맨 마지막 부분 제거 여부에 관한 flag이다 
		// -> 실제로 이동을 하지 않거나, 파일을 선택한 경우에는 prevflag = 1로하여 맨 마지막을 제거
		// -> 이동할 디렉토리를 선택한 경우 마지막을 제거하지 않고 그대로 진행
		if(prevflag == 1){
			// 연결리스트의 마지막부분을 지우고 이동된 현재 디렉토리의 이름을 만들어 낸다.
			removeList(pointer);
			makedirname(pointer, targetdir);
		}	

		// dirManager는 targetdir의 디렉토리 내에 존재하는 파일들을 출력해주는 함수이다
		// 내부에서 선택된 메뉴를 return하여 아래에서 기능을 수행한다
		choice = dirManager(targetdir, dirname);

		// 접근할 수 없는 디렉토리이거나, 파일/디렉토리 선택을 취소한 경우
		// 위로 돌아가서 dirManager를 다시 수행한다
		if(choice == -1)
			continue;
		
		// 시스템 종료를 선택한 부분 ( 반복문을 빠져나가서 프로그램을 종료한다 )
		if(choice == 0)
			break;
			

		// dirManager에서 dirname( 선택한 파일 명)을 처리하는 부분
		if(strcmp(dirname, "/..") == 0){		// 상위 디렉토리를 선택한 경우
			// 홈 디렉토리에서 선택했으면 더 이상 이동하지 못하도록 한다 ( 붙여넣기 상태 제외 )
			if(strcmp(targetdir, pw->pw_dir) == 0 && choice != 6){	
				printf("더 이상 이동할 수 없습니다\n");
				continue;
			} else{
				// 홈 디렉토리가 아니면 연결리스트의 맨 마지막을 제거하여 상위 디렉토리를 얻는다
				if(choice != 6)
					removeList(pointer);

				// 붙여넣기 상태에서는 일단 /..을 추가한 뒤 아래 동작에서 remove한다
				else
					addList(pointer, dirname);
			}
		}
		else if(strcmp(dirname, "/.") == 0){		// 현재 디렉토리를 선택한 경우
			// 정보 확인을 제외한 나머지 경우에 dirManager를 다시 호출한다.
			if(choice != 1)	
				continue;
		}
		else						// 그 외 나머지 파일은 일단 리스트에 추가한다.
			addList(pointer, dirname);

		// 추가된 리스트를 바탕으로 새로운 절대 경로를 만들어낸다
		makedirname(pointer, targetdir);


		// 기능 동작 부분
		switch(choice){
			// 파일, 디렉토리 정보 출력
			case 1:
				// 파일 정보를 출력하는 함수
				fileInfo(targetdir);

				// /.를 선택한 경우가 아니면 prevflag = 1로 바꿈
				if(strcmp(dirname, "/.") != 0){
					prevflag = 1;
				}
				break;
			// 파일 실행 / 디렉토리 이동
			case 2:
				if(prevflag == 1){		// 파일을 선택한 경우 해당 파일을 수행한다
					printf("%s 파일 실행\n", targetdir);
					system(targetdir);
				}
				// 디렉토리 이동은 prevflag = 0으로 설정하여 dirManager에서 이동
				break;

			// vi로 파일 편집
			case 3:
				openfile(targetdir); // 파일 실행 함수
				break;
			// 파일, 디렉토리 삭제
			case 4:
				deletefile(targetdir);	// 파일 삭제 함수
				// 디렉토리나 파일을 삭제한 경우 해당 파일은 존재하지 않기 때문에 리스트에서 제거
				removeList(pointer);
				makedirname(pointer, targetdir);
				break;
			// 파일, 디렉토리 복사
			case 5:
				// 복사한 파일 경로를 sourcedir에 저장
				strcpy(sourcedir, targetdir);
				printf("%s 파일을 복사했습니다 \n", sourcedir);
				copyflag = 1;
				prevflag = 1;
				break;
			// 파일, 디렉토리 붙여넣기
			case 6:
				// 위에서 추가한 /..을 지운다, /..가 아닌 경우에는 이전에 선택한 경로가 또 추가되기 때문에
				// 마찬가지로 지운다
				removeList(pointer);			
				makedirname(pointer, targetdir);
				copypastefile(sourcedir, targetdir); // 파일 복사 함수
				break;
			// 파일 생성
			case 7:
				// case 6과 동일하다
				removeList(pointer);
				makedirname(pointer, targetdir);
				createfile(targetdir);	// 파일 생성 함수
				break;
			// 디렉토리 생성
			case 8:
				removeList(pointer);
				makedirname(pointer, targetdir);
				createdir(targetdir);	// 디렉토리 생성 함수
				break;
		}

		system("sleep 0.5");	// 동작 확인을 위한 sleep
				
	}

	
	return 0;

}


// 리스트에 존재하는 파일명들을 합쳐서 하나의 절대 경로를 만드는 함수
void makedirname(List *head, char *dirname){
	strcpy(dirname, head->dirname);		// 홈 디렉토리를 추가하고
	List *target = head->nextdir;
	while(target != head){			// 리스트를 탐색하면서 strcat으로 추가
		strcat(dirname, target->dirname);
		target = target->nextdir;
	}
}

// 선택한 파일을 vi로 연다
void openfile(char * filename){
	char command[MAX_LENGTH];

	// 파일을 선택한 경우
	if(prevflag == 1){
		strcpy(command, "vi ");
		strcat(command, filename);
		system(command);
	} 
	// 디렉토리를 선택한 경우
	else{
		printf("디렉토리는 편집할 수 없습니다\n");
		prevflag = 1;
	}
}

// 선택한 파일을 삭제한다
void deletefile(char *filename){
	char choice;
	char command[MAX_LENGTH];

	// 삭제여부를 한번 더 물은 뒤 지운다
	printf("정말 %s 을 삭제하시겠습니까? (y | n) >> ", filename);
	scanf(" %c", &choice);
	if(choice == 'y'){
		strcpy(command, "rm -rf ");
		strcat(command, filename);
		system(command);
		printf("%s 파일 삭제 완료\n", filename);
	} else{
		printf("삭제 취소합니다\n");
	}

	// case에서 리스트를 하나 지우기 때문에 위에서 지울 필요가 없으므로 바꿈
	if(prevflag == 1)
		prevflag = 0;
}

// 파일 복사 및 붙여넣기
void copypastefile(char *src, char *dst){
	char command[MAX_LENGTH];

	strcpy(command, "cp ");
	strcat(command, src);
	strcat(command, " ");
	strcat(command, dst);
	system(command);		// command 명렁어를 만든 뒤 system으로 수행한다
	copyflag = 0;
	prevflag = 0;
	printf("파일 붙여넣기 완료\n");
}


// 파일 생성
void createfile(char *dirname){
	char create[MAX_LENGTH];
	char filename[MAX_LENGTH];
	int fd;
	
	prevflag = 0;

	// 생성할 파일 명을 입력받은 뒤
	printf("생성할 파일 명 입력 >> ");
	scanf(" %s", filename);
	while(getchar() != '\n');

	// 현재 위치의 절대경로와 파일명을 합쳐서 파일 이름을 생성한다
	strcpy(create, dirname);
	strcat(create, "/");
	strcat(create, filename);

	// 파일 생성 부분
	fd = open(create, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(fd == -1){
		printf("파일 생성 실패\n");
		return;
	}

	close(fd);
	printf("파일 생성 완료\n");
}


// 디렉토리 생성
void createdir(char * dirname){
	char create[MAX_LENGTH];
	char filename[MAX_LENGTH];
	int dfd;

	prevflag = 0;

	// 파일 생성과 마찬가지로 이름을 입력받고
	printf("생성할 디렉토리 명 입력 >> ");
	scanf(" %s", filename);
	while(getchar() != '\n');

	// 디렉토리 이름을 절대경로로 만든 뒤
	strcpy(create, dirname);
	strcat(create, "/");
	strcat(create, filename);

	// 디렉토리를 생성한다
	dfd = mkdir(create, 0755);
	if(dfd == -1){
		printf("디렉토리 생성 실패\n");
		return;
	}
	printf("디렉토리 생성 완료\n");
}
