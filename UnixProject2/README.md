# Unix-Project2
> 유닉스 프로그래밍 프로젝트 2

## 1. 주제
 - 서버측에서 동작하는 시스템으로 서버와 연결된 클라이언트들의 자원 ( 프로세스 정보, 디스크 공간 및 마운트 정보, 클라이언트 시스템 정보 ) 들을 실시간으로 확인할 수 있도록 제공하는 시스템 입니다. 서버의 경우 특정한 상황을 제외하고는 항상 동작해야 하기 때문에 서버의 자원 관리 및 상시 확인이 필요할 수 있습니다. 서버의 상태를 쉽게 확인하기 위해서 각 서버들이 해당 시스템의 클라이언트로서 연결되어 있으며 각 서버들의 자원 정보를 출력해주는 기능을 지원합니다. 

## 2. 시스템 설계도
> 
 ![image](https://user-images.githubusercontent.com/60842206/130898845-a830fb40-55f9-46ac-921e-a72403c041b4.png)

- 시스템을 시작하면 main 함수와 두 개의 스레드를 통해서 시스템이 동작합니다. 먼저 메인함수는 GUI 위젯을 동작하는 함수로 소켓 연결과, 서브 윈도우 생성 스레드를 만들고 gtk_main을 동작시킵니다. 소켓 연결을 하는 socket_main은 여러 개의 클라이언트를 받아야 하기 때문에 main_handler를 동작시키는 최대 4개의 스레드를 생성하여 연결과 데이터 전송, 받는 것만 전담합니다. 마지막으로 서브윈도우 생성은 각 클라이언트 데이터와 매핑되어 4개의 서브윈도우가 스레드로 동작하고 서브윈도우에 존재하는 버튼의 시그널을 기다리고 있습니다.
## 3. 사용된 기술
- 소켓 프로그래밍 : 서버와 클라이언트를 연결하는 소켓 프로그래밍을 사용. netinet/in arpa/inet.h, sys/socket.h 를 사용해서 구현하였습니다.
- 스레드 : GUI위젯과 서버, 클라이언트 연결이 별개로 동작해야 하기 때문에 pthread.h를 사용하여 스레드를 구현하였습니다.
- 시그널 : GUI 버튼, 클라이언트 접속 종료 등에 사용했으며 while문을 통해 동작하는 코드에서 시그널을 이용해 break할 수 있도록 하기 위해 사용하였습니다.
- Gtk 3.0 : GUI위젯을 구현하기 위한 외부 라이브러리 입니다. 
- Glade : GUI위젯을 보다 편리하게 구현할 수 있도록 하는 도구입니다.

## 4. 사용 매뉴얼
- Gtk3.0 라이브러리가 설치되어 있어야 사용가능한 시스템 입니다. ( 우분투 환경 기준 sudo apt-get install gtk3.0 명령어 사용 ) 
- 서버 프로그램 컴파일 시 make guiserver 명령, 클라이언트 프로그램 컴파일 시 make guiclient 명령을 이용하면 됩니다.
① 시스템 시작 
- 서버 : 서버측 프로그램인 ServerProgram을 실행하면 클라이언트 연결을 기다리는 상태를 유지하고 GUI Program이 실행됩니다.
- 클라이언트 : 클라이언트측 프로그램인 ClientProgram을 실행하면 서버와 연결을 시도하며 접속 성공 시 완료메시지를 출력합니다. 클라이언트는 최대 4개까지만 연결할 수 있습니다.
② Main Controller
- 4개의 Client 버튼과 1개의 Close 버튼이 존재합니다.
(1) Client 버튼 : Client Information 이라는 새로운 창을 생성합니다.
(2) Close 버튼 : Main Controller를 닫으며 프로그램을 종료합니다. ( 연결된 클라이언트들도 전부 종료됩니다. )
③ Client Information
- 4개의 버튼이 존재합니다. 각 버튼은 연결되지 않은 경우 Not Connected 메시지를 출력합니다.
(1) Process 버튼 : 클라이언트의 프로세스 정보를 출력합니다. 유닉스 터미널 명령 ps –ef 와 같은 동작입니다.
(2) System Info 버튼 : 클라이언트의 시스템 정보를 출력합니다. 유닉스 시스템 정보, 버전, 및 CPU정보를 출력합니다.
(3) Disk Info 버튼 : 클라이언트 디스크 및 마운트 정보를 출력합니다. 유닉스 터미널 명령 df –h와 같은 동작입니다.
(4) Close 버튼 : Client Information 창을 닫습니다.
④ 시스템 종료
- 서버 : ②의 설명대로 Close 버튼이나 x버튼을 누르면 프로그램이 종료됩니다.
- 클라이언트 : 서버에 접속한 후 클라이언트는 어떠한 동작도 되지 않습니다. Ctrl + C 키를 이용해서 프로그램을 종료할 수 있습니다.

## 5. 사용 예
① 시스템 컴파일 및 실행
- gtk 라이브러리 설치

![image](https://user-images.githubusercontent.com/60842206/130898952-8602ee25-100f-456c-b036-f57ab9f2546f.png)

- 서버 측 컴파일
 
 ![image](https://user-images.githubusercontent.com/60842206/130899044-2a3a1341-c358-4860-8005-2fccf83151fe.png)

- 클라이언트 측 컴파일
 
 ![image](https://user-images.githubusercontent.com/60842206/130899049-0fb944c6-5878-4687-a880-791be74a22f0.png)


② 시스템 시작
- 서버 측 시스템 시작
 
![image](https://user-images.githubusercontent.com/60842206/130899143-8d8233ef-cd74-4d9b-911e-ea414288847c.png)

- 클라이언트 측 시스템 시작
 
 ![image](https://user-images.githubusercontent.com/60842206/130899153-50a97896-6923-401b-a085-0506028179f1.png)


- 클라이언트 접속 완료 시 서버 측 화면  

![image](https://user-images.githubusercontent.com/60842206/131338052-15f6ca8e-cfeb-474e-92f8-4ec45632aa4d.png)

 
③ Client Information 화면 동작
- 기본 화면 출력

![image](https://user-images.githubusercontent.com/60842206/131338080-ac9f2e16-1fba-4d91-a49e-7052c704f25a.png)

 
- Process 버튼 클릭 시

![image](https://user-images.githubusercontent.com/60842206/131338099-eaf69694-8343-4eec-9679-ea8affbf5f51.png)

 
- System Info 버튼 클릭 시

![image](https://user-images.githubusercontent.com/60842206/131338114-3a3b4a36-8279-41cc-bafb-3951bbe2b6dc.png)

 
- Disk Info 버튼 클릭 시

![image](https://user-images.githubusercontent.com/60842206/131338141-95f0086a-12f1-430c-80e2-2df411430297.png)

 
- 클라이언트가 종료된 경우 / 연결되지 않은 경우

![image](https://user-images.githubusercontent.com/60842206/131338160-e4d0f352-ded9-440b-b377-fcb60fa633b3.png)

 
- 클라이언트 측 시스템 종료 ( Ctrl + C 를 이용함 )
