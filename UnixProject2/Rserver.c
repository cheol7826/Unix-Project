#include "Rserver.h"
#include <pthread.h>
#define PORTNUM 9888
#define BUFSIZE 1024


void* main_handler(void* soc);
int clients[MAX_CLIENT];
int curClient;
int thptr;
pthread_mutex_t mutex;
pthread_t socketth[MAX_CLIENT];


// 클라이언트 소켓 연결 함수
void socketmain(){
	char buf[1024];
	struct sockaddr_in sin, cli;
	int len = sizeof(cli);
	int sd, cd;
	pid_t pid;
	char ip[256];
	FILE *fp;

	// hostname -I 명령으로 현재 호스트( 서버 ) 의 ip주소를 얻음
	fp = popen("hostname -I", "r");
	fgets(ip, sizeof(ip), fp);
	pclose(fp);

	// 서버 소켓을 생성 ( 에러발생 시 메시지 출력 후 종료 )
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	// 스레드에서 사용할 mutex lock 초기화
	pthread_mutex_init(&mutex, NULL);

	// 서버 소켓 설정 ( TCP, 포트번호, IP주소 설정 )
	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr(ip);

	// 위에서 설정한 부분을 소켓에 적용 (마찬가지로 에러 발생지 메시지 출력 후 종료)
	if(bind(sd, (struct sockaddr*)&sin, sizeof(sin))) {
		perror("bind");
		exit(1);
	}


	// 클라이언트 접속 받는 부분
	while(1) {
		// 클라이언트의 접속 요청을 기다림
		if(listen(sd, 0) == -1){
			perror("listen");
			exit(1);
		}

		// 클라이언트가 연결되면 소켓 디스크립터를 반환한다
		if((cd = accept(sd, (struct sockaddr*) &cli, &len)) == -1){
			perror("accept");
		}

		// 연결된 클라이언트는 스레드로 생성하여 동작한다
		// 스레드 생성 후 다시 위로 올라가서 accept를 기다린다
		pthread_mutex_lock(&mutex);				// 동기화 문제 해결을 위해 lock을 건다
	        while(1){	
			if(clients[curClient] == 0)			// clients 배열은 클라이언트 접속 여부를 알 수 있는 변수로
				break;					// 소켓 디스크립터가 들어있다 ( 0인경우 비어있는 상태 )
			curClient = (curClient+1) % MAX_CLIENT;		// 빈 공간을 찾을때 까지 순환한다
			thptr = (thptr+1) % MAX_CLIENT;
		}
		clients[curClient] = cd;				// 공간을 찾은경우 소켓 디스크립터를 저장하고
		connected[curClient] = 1;				// 현재 클라이언트 접속여부 표시 후 
		curClient = (curClient+1);				// unlock한다
		thptr = (thptr+1) % MAX_CLIENT;
		pthread_mutex_unlock(&mutex);

		printf("Client %d connected\n", curClient);			// 연결 알림 메시지
		
		// 스레드 생성 후 main_handler를 통해서 데이터를 얻어온다
		pthread_create(&socketth[thptr], NULL, main_handler, (void*) &curClient);
		pthread_detach(socketth[thptr]);		// 메인 스레드에 의존하지 않기 때문에 detach를 통해서 분리한다.
				
	}
	close(sd);
}


// 스레드에서 동작하는 함수
void* main_handler(void* soc){
	int target = *((int *)soc) - 1;
	char buf[256];
	int c;

	// getdata는 클라이언트에서 받아온 데이터를 저장하는 변수
	strcpy(getdata[target], "");

	// 스레드를 생성했으면 연결이 완료된 것으로 간주하고 클라이언트에게 메시지를 보냄
	// 클라이언트는 이 메시지를 받기 전까지 접속 대기상태를 유지함
	send(clients[target], "\0", 10, 0);

	// 클라이언트에 전송할 명령어 집합
	// 프로세스 정보, 시스템 정보, 디스크 정보
	char cmd[4][256] = {"nothing\0", "ps -ef\0", "cat /proc/version ; cat /proc/cpuinfo\0", "df -h\0"};


	// 클라이언트에서 데이터를 받아오는 부분
	for(;;){
		// if문에는 처음에만 들어가고 GTK에서 다른 동작이 있지 않는 이상 들어가지 않는다
		// 두번째 이후는 GTK에서 active값을 변경할 때 아래 코드가 동작한다
		if(active[target] == 0 || (flag[target] != 0 && active[target] == 0)){
			send(clients[target], cmd[flag[target]], 256, MSG_NOSIGNAL);	// 설정한 명령어 전송
			active[target] = 1;						// 이후 다른 조작없이 들어오지 않도록 설정
		
			// 데이터를 한번에 받을 수 없기 때문에 여러번 recv하여 받는다
			while(1){
				// recv 결과가 0인 경우는 클라이언트와 연결이 해제된 경우로 스레드 종료 처리한다.
				if((c = recv(clients[target], buf, sizeof(buf), 0)) == 0){
					strcpy(getdata[target], "Not Connected\0");
					curClient = (curClient+3) % MAX_CLIENT;
					close(clients[target]);
					connected[target] = 0;
					clients[target] = 0;
					printf("Client %d exit\n", target+1);
					pthread_exit(NULL);
				}
				// end가 전송되면 명령어 전송을 완료한 경우로 반복문을 탈출한다
				if(!strcmp(buf, "end")){
					break;
				}
				printf("%s", buf);
				// 그렇지 않은 경우는 명령어와 관련된 데이터가 전송된 것으로 getdata에 저장한다
				strcat(getdata[target], buf);
				strcpy(buf, "");
			}

		}
	}
	
}
