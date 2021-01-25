#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTNUM 9888
int sd;

// 클라이언트 프로그램 종료시 사용하는 시그널
// ctrl + C 를 통해서 종료한다
void sighandler(int signo){
	printf("System exit\n");
	close(sd);
	exit(1);
}

int main(void){
	char buf[256];
	struct sockaddr_in sin;
	FILE *fp;
	char ip[256];

	// 접속할 서버의 IP주소를 입력
	printf("접속할 서버 IP주소 입력 >> ");
	scanf("%s", ip);
	
	// 클라이언트 소켓 생성
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	// 소켓 구조체를 통해서 소켓 정보를 설정 ( IP, 포트, TCP로 설정)
	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr(ip);

	// 설정한 소켓을 통해서 서버에 연결 ( 서버에 접속할 수 없는 경우 에러메시지 출력 후 종료 )
	if(connect(sd, (struct sockaddr *)&sin, sizeof(sin)) == -1){
		printf("서버에 접속할 수 없습니다\n");
		exit(1);
	}

	// 서버에서 스레드 생성 후 명령어를 전달하기 전까지 클라이언트는 대기상태
	printf("서버 접속 대기중\n");

	// 서버에서 명령어를 보내기 전 메시지를 전송한 경우 접속 완료 메시지를 출력한다
	recv(sd, buf, 10, 0);
	printf("서버 접속 완료\n");

	// interrupt 시그널을 통해 프로그램 종료 시그널 생성
	signal(SIGINT, sighandler);
	
	char command[256];

	for(;;){
		while(1){
			// 서버에서 전송한 명령어를 받는다
			recv(sd, command, sizeof(command), 0);
			if(!strcmp(command, "nothing")){	// 맨 처음에는 nothing이 전송되기 때문에 end를 전송하고
				send(sd, "end\0", 10, 0);	// recv로 돌아가서 명령을 기다린다.
			} 	
			else{					// nothing이 아닌 경우 반복문을 탈출하여 다음 명령을 수행한다
				break;
			}
		}

		// 받은 명령을 파이프를 통해서 연다
		if((fp = popen(command, "r")) == NULL){
			printf("command is not found\n");
		}
	
		// 파이프를 통해 얻은 정보를 서버로 전송한다
		while(1){
			// 데이터를 전부 받은경우 end를 전송하여 서버측 반복문을 탈출하도록 한다.
			if(fgets(buf, sizeof(buf), fp) == NULL){
				send(sd, "end\0", 5, 0);
				break;
			}
			
			// 데이터가 있는경우 데이터를 서버로 전송
			if(send(sd, buf, sizeof(buf), 0) == -1){
				perror("send");
				break;
			}
		}
		// 파이프를 닫고 다음 명령을 기다린다.
		pclose(fp);
	}
	return 0;
}

