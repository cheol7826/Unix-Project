#include <gtk/gtk.h>
#include <string.h>
#include <glib.h>
#include <pthread.h>
#include "Rserver.h"

typedef struct thinfo{
	int threadnum;
	int buttonnum;
}Info;

extern int active[MAX_CLIENT];
extern int flag[MAX_CLIENT];
extern char getdata[4][MAX_LENGTH];
extern int connected[MAX_CLIENT];

pthread_t windowthread[MAX_CLIENT];
Info in[MAX_CLIENT][4];

GtkWidget * textview[MAX_CLIENT];
GtkWidget * subwindow;

// 클라이언트 정보 창에서 Close 버튼을 누를 시 동작되는 함수
void close_window(GtkWidget *w, gpointer data){
	// 클라이언트 정보 창을 닫는다
	gtk_widget_destroy(data);
}

// 클라이언트 정보 창의 텍스트 부분을 만들어주는 함수
void create_text(GtkWidget * textview, char* t){
	GtkTextBuffer * text;

	// GtkTextView는 GtkTextBuffer에 문자열을 저장하여 View에 출력하는 형태이다
	text = gtk_text_buffer_new(NULL);

	// 문자열을 넘겨받아 TextBuffer에 저장하고
	gtk_text_buffer_set_text(text, t, -1);
	// TextView에 설정한다
	gtk_text_view_set_buffer((GtkTextView*)textview, text);

}

// 클라이언트 정보 창에서 Process, System, Disk, Close 버튼을 누를 시 동작하는 함수
void* set_flags(GtkWidget *w, gpointer data){
	Info value = *(Info *)data;

	// 버튼을 누른 클라이언트의 번호와 버튼 번호를 저장
	int cli = value.threadnum;
	int target = value.buttonnum;

	flag[cli] = target;		// 명령어의 종류를 결정하는 부분
	active[cli] = 0;		// if문에 다시 들어갈 수 있도록 설정
	sleep(1);			// 충분한 데이터를 받아오기 위해 잠시 sleep한다.


	// Close버튼을 누른 것이 아니라면
	if(value.buttonnum != 4){			
		create_text(textview[cli], getdata[cli]);		// 위의 TextView를 설정하는 함수 호출
		gtk_widget_show_all(subwindow);				// 설정된 TextView를 적용하기 위해 호출하는 함수

		if(connected[cli] != 0)					// 받은 데이터를 계속 유지하지 않고 초기화
			strcpy(getdata[cli], "");
	}
}

// 클라이언트 정보 창을 생성하는 함수
void* create_subwindow(void * n){
	int threadnum = *(int *)n;
	char clientnum[9];
	GtkBuilder * builder;
	GtkWidget * exitbtn;
	GtkWidget *button[3];
	GtkWidget *clientlabel;

	
	// Glade라는 위젯 설정 프로그램에서 만든 glade파일로 각 위젯의 정보를 얻는다
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "guisub.glade", NULL);

	// GtkWindow에 해당하는 정보를 파일에서 얻는다, 
	subwindow = (GtkWidget *)gtk_builder_get_object(builder, "workWindow");

	// 클라이언트 창 번호 설정 부분
	strcpy(clientnum, "Client ");
	clientnum[7] = '0' + threadnum + 1;
	clientnum[8] = '\0';
	clientlabel = (GtkWidget *)gtk_builder_get_object(builder, "windownumber");
	gtk_label_set_text((GtkLabel * )clientlabel, clientnum);

	// Close버튼에 대한 정보를 얻는다	--> 위에서 동작하는 함수들은 g_connect_signal 함수를 통해서 시그널처럼 동작한다
	exitbtn = (GtkWidget *)gtk_builder_get_object(builder, "exitbtn");
	// 버튼 클릭한 경우 close_window 함수를 호출한다
	g_signal_connect(exitbtn, "clicked", G_CALLBACK(close_window), subwindow);

	// GtkTextView 위젯 정보를 얻어오는 부분 ( 현재 클라이언트 정보 창으로 얻어온다 )
	textview[threadnum] = (GtkWidget *)gtk_builder_get_object(builder, "textview");


	// 각 명령의 기능을 담당하는 부분 
	button[0] = (GtkWidget *)gtk_builder_get_object(builder, "processbtn");
	button[1] = (GtkWidget *)gtk_builder_get_object(builder, "systembtn");
	button[2] = (GtkWidget *)gtk_builder_get_object(builder, "diskbtn");

	// 각 버튼 클릭 시 동작하는 함수 설정
	for(int i=0; i<3; i++){
		in[threadnum][i].threadnum = threadnum;
		in[threadnum][i].buttonnum = i+1;
		g_signal_connect(button[i], "clicked", G_CALLBACK(set_flags), (void*)&in[threadnum][i]);
									
	}

	// Close버튼에 대한 함수 설정 ( Close 버튼을 누르는 경우 두 가지 함수가 동작한다 )
	in[threadnum][3].threadnum = threadnum;
	in[threadnum][3].buttonnum = 4;
	g_signal_connect(exitbtn, "clicked", G_CALLBACK(set_flags), (void*)&in[threadnum][3]);

	// 각 기능들이 전부 수행되고 나면 해당 함수를 통해 subwindow 내부에 존재하는 모든 위젯들을 표시한다.
	gtk_widget_show_all(subwindow);
}

// 클라이언트 정보 창 각각을 스레드로 만들어서 동작하게 한다
void thread_subwindow(GtkWidget *w, gpointer data){
	int thdptr = *(int *)data;
	pthread_create(&windowthread[thdptr], NULL, create_subwindow, (void*) &thdptr);
	pthread_join(windowthread[thdptr], 0);
}



// GTK Widget 메인 함수
int main(int argc, char *argv[]) {
        GtkBuilder *builder;
        GtkWidget *window;
	GtkWidget *exitbtn;
	GtkWidget *button[4];
	int btnptr[4];
	pthread_t mainthread;

	// 처음 실행 시 getdata를 설정한다 ( 연결되지 않은 상태에서 클라이언트 정보창에는 다음과 같이 표시한다 )
	for(int i=0; i<MAX_CLIENT; i++){
		strcpy(getdata[i], "Not Connected\0");
	}
	
	// 클라이언트를 연결하는 부분을 스레드로 생성한다
	pthread_create(&mainthread, NULL, (void*)socketmain, NULL);
	pthread_detach(mainthread);

	// GTK 초기화 함수 
        gtk_init(&argc, &argv);

	// 메인 위젯을 불러오는 부분( 위의 방법과 동일하다 ) --> glade를 이용
        builder = gtk_builder_new();

        gtk_builder_add_from_file(builder, "guimain.glade", NULL);

	// 메인 위젯 윈도우를 얻어온 후 signal 설정
        window = (GtkWidget *)gtk_builder_get_object(builder, "mainWindow");
	// x 버튼을 누른 경우에 프로그램이 종료되도록 한다.
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Close버튼도 signal 설정, 마찬가지로 누를 경우 프로그램이 종료되도록 한다
	exitbtn = (GtkWidget*) gtk_builder_get_object(builder, "exitbtn");
	g_signal_connect(exitbtn, "clicked", G_CALLBACK(gtk_main_quit), NULL);

	// 4개의 클라이언트 정보를 표시할 수 있는 버튼 ( 누를 시 클라이언트 정보 창이 생성된다 )
	button[0] = (GtkWidget *)gtk_builder_get_object(builder, "psbtn1");
	button[1] = (GtkWidget *)gtk_builder_get_object(builder, "psbtn2");
	button[2] = (GtkWidget *)gtk_builder_get_object(builder, "psbtn3");
	button[3] = (GtkWidget *)gtk_builder_get_object(builder, "psbtn4");

	// 클라이언트 정보 창을 생성하는 signal 설정
	for(int i=0; i<4; i++){
		btnptr[i] = i;
		g_signal_connect(button[i], "clicked", G_CALLBACK(thread_subwindow), (void *)&btnptr[i]);
	}

	// 메인 위젯 윈도우 표시하는 부분
        gtk_widget_show_all(window);

	// 지금까지 설정한 GTK 위젯 설정들을 실행하는 함수
        gtk_main();

        return 0;

}

