#include "directory.h"
#include "print.h"


struct dirent** list;

void permcheck(char perm[][4], mode_t mode);

int dirManager(char* dirname, char* choicedir){
	int i;
	int func, dirnum;

	i = 0;

	if((i = scandir(dirname, &list, NULL, alphasort)) == -1){
		printf("디렉토리에 접근할 수 없습니다\n");		// 디렉토리 접근 불가능
		return -1;
	}

	printf("-----------------------------------------");
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

void fileInfo(char * filename){
	struct stat info;

	char perm[3][4];
	for(int i=0; i<3; i++)
		perm[i][3] = '\0';

	if(stat(filename, &info) == -1){
		printf("파일 정보를 불러올 수 없습니다\n");
		return;
	}

	permcheck(perm, info.st_mode);

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

void permcheck(char perm[][4], mode_t mode){
	mode_t mask[3][3];
	mode_t setmask[3];

	char rwx[3] = {'r', 'w', 'x'};

	mask[0][0] = S_IRUSR;
	mask[0][1] = S_IWUSR;
	mask[0][2] = S_IXUSR;
	mask[1][0] = S_IRGRP;
	mask[1][1] = S_IWGRP;
	mask[1][2] = S_IXGRP;
	mask[2][0] = S_IROTH;
	mask[2][1] = S_IWOTH;
	mask[2][2] = S_IXOTH;

	setmask[0] = S_ISUID;
	setmask[1] = S_ISGID;
	setmask[2] = S_ISVTX;

	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			if((mode & mask[i][j])){
				perm[i][j] = rwx[j];
			} else{
				perm[i][j] = '-';
			}
		}
	}

	for(int i=0; i<3; i++){
		if((mode & setmask[i])){
			if(perm[i][2] == rwx[2])
				perm[i][2] = (i == 2) ? 't' : 's';
			else
				perm[i][2] = (i == 2) ? 'T' : 'S';
		}
	}
}









