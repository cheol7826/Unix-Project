#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENT 4
#define MAX_LENGTH 20000

void socketmain();
int flag[MAX_CLIENT];
char getdata[4][MAX_LENGTH];
int connected[MAX_CLIENT];
int active[MAX_CLIENT];
