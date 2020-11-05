#include "directory.h"
#include "print.h"


struct dirent** list;

void permcheck(char perm[][4], mode_t mode);

int dirManager(char* dirname, char* choicedir){
	int i;
	int func, dirnum;

	i = 0;

	// Scandir을 통해서 디렉토리 엔트리를 알파벳 순으로 정렬해서 얻는다
	if((i = scandir(dirname, &list, NULL, alphasort)) == -1){
		printf("디렉토리에 접근할 수 없습니다\n");	// 디렉토리에 접근이 불가능한 경우 -1 리턴
		prevflag = 0;
		return -1;
	}

	printf("-----------------------------------------");
	printf("-----------------------------------------\n");

	// scandir로 얻은 디렉토리 엔트리를 통해 디렉토리 파일 목록을 출력하는 부분
	for(int k=0; k<i; k++){
		printf("%3d. ", k+1);
		// 현재 엔트리가 디렉토리인 경우 파란색 이름으로 출력
		if(list[k]->d_type == DT_DIR)
			printf("\x1b[34m");
		printf("%s\n", list[k]->d_name);
		printf("\x1b[0m");
	}

	// 실제 기능을 수행할 메뉴를 출력하고 선택하는 부분
	func = printMainMenu(dirname);
	if(func == 0){
		printf("시스템을 종료합니다.\n");
		return func;
	}

	// 6, 7, 8번은 파일을 선택하지 않는다
	if(func < 6){	
		dirnum = printDirMenu(i);	// 파일 선택 함수
		
		if(dirnum == 0){		// 파일 선택을 취소한 경우
			prevflag = 0;
			return -1;
		}

		// 파일, 디렉토리에 대한 prevflag를 설정
		if(list[dirnum-1]->d_type == DT_DIR){
			prevflag = 0;
		} else {
			prevflag = 1;
		}
		
		// 선택한 파일의 이름을 생성
		strcpy(choicedir, "/");
		strcat(choicedir, list[dirnum-1]->d_name);
	}

	return func;
}


// 파일 정보를 출력하는 함수
void fileInfo(char * filename){
	struct stat info;

	// 파일 권한 체크를 위한 배열 ( r, w, x, - 를 추가하여 문자열로 처리 )
	char perm[3][4];
	for(int i=0; i<3; i++)
		perm[i][3] = '\0';

	// 심볼릭 링크 파일 처리를 위해 lstat으로 받는다
	if(lstat(filename, &info) == -1){
		printf("파일 정보를 불러올 수 없습니다\n");
		return;
	}

	// 선택한 파일의 권한을 체크 ( perm 배열에 저장 )
	permcheck(perm, info.st_mode);

	// 파일 정보 출력
	printf("-----------------------------------------");
	printf("-----------------------------------------\n");
	printf("- 파일 이름            : %s\n", filename);
	printf("- 파일 종류            : ");
	switch (info.st_mode & S_IFMT) { 
		case S_IFBLK: 
			printf("장치 파일(block)\n"); 
			break; 
		case S_IFCHR: 
			printf("장치 파일(character)\n"); 
			break; 
		case S_IFDIR: 
			printf("디렉토리\n"); 
			break; 
		case S_IFIFO: 
			printf("FIFO 파일\n"); 
			break; 
		case S_IFLNK: 
			printf("심볼릭 링크 파일\n"); 
			break; 
		case S_IFREG: 
			printf("일반 파일\n"); 
			break; 
		case S_IFSOCK: 
			printf("소켓 파일\n"); 
			break; 
		default: 
			printf("알 수 없는 파일\n"); 
			break; 
	}

	printf("- Inode 번호           : %ld\n", (long)info.st_ino);
	printf("- 소유자/소유그룹      : UID = %ld, GID = %ld\n", (long)info.st_uid, (long)info.st_gid);
	printf("- 파일 권한   	       : User : %s, Group : %s, Others : %s\n", perm[0], perm[1], perm[2]);
	printf("- 파일 크기            : %lld bytes\n", (long long)info.st_size);
	printf("- 하드링크 파일 수     : %ld 개\n", (long)info.st_nlink);
	printf("- Access 시간          : %s", ctime(&info.st_atime));
	printf("- Modified 시간        : %s", ctime(&info.st_mtime));
	printf("- Change Status 시간   : %s", ctime(&info.st_ctime));
	printf("-----------------------------------------");
	printf("-----------------------------------------\n");
}


// 파일 권한 체크 함수
void permcheck(char perm[][4], mode_t mode){
	mode_t mask[3][3];
	mode_t setmask[3];

	char rwx[3] = {'r', 'w', 'x'};

	// 권한을 확인할 mask를 배열에 저장
	mask[0][0] = S_IRUSR;
	mask[0][1] = S_IWUSR;
	mask[0][2] = S_IXUSR;
	mask[1][0] = S_IRGRP;
	mask[1][1] = S_IWGRP;
	mask[1][2] = S_IXGRP;
	mask[2][0] = S_IROTH;
	mask[2][1] = S_IWOTH;
	mask[2][2] = S_IXOTH;

	// setuid, setgid, sticky bit 확인
	setmask[0] = S_ISUID;
	setmask[1] = S_ISGID;
	setmask[2] = S_ISVTX;

	// 일반 rwx 권한을 확인하는 부분
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			if((mode & mask[i][j])){	// 마스크와 동일하면 해당 권한을 추가하고
				perm[i][j] = rwx[j];
			} else{				// 아니면 - 를 추가한다
				perm[i][j] = '-';
			}
		}
	}

	// 특수 권한을 확인하는 부분
	for(int i=0; i<3; i++){
		if((mode & setmask[i])){
			if(perm[i][2] == rwx[2])	// x권한이 있으면 s나 t로 설정하고
				perm[i][2] = (i == 2) ? 't' : 's';
			else				// x권한이 없으면 S나 T로 설정한다
				perm[i][2] = (i == 2) ? 'T' : 'S';
		}
	}
}









