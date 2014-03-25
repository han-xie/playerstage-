/*
 * main.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: keevi7c3l
 */

#include "apue.h"
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h> // fork, close
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include </usr/include/pthread.h>

#define MAXLINE 4096
#define DEBUGV 1
#define MAXWAITTIME 5

int inputNumber = 0;
int exitStatus = 0;
char commandline[4096];
pthread_t ntid;
pthread_mutex_t globalLock;
int listenfd, connfd;
struct sockaddr_in servaddr;
char buff[4096];
int sockfdc;
char recvlinec[4096];
//char sendlinec[4096];
struct sockaddr_in servaddrc;

/*below all about  buffer*/
#define MAXBUFFERBLOCKUNMBER 20
#define OCCUPY 0           //'\0'
#define UNOCCUPY 1         //'\x01'
pthread_mutex_t mutexOccupyTable = PTHREAD_MUTEX_INITIALIZER;
char occupyT[MAXBUFFERBLOCKUNMBER];
int sock[MAXBUFFERBLOCKUNMBER];
pthread_mutex_t mutexBuffer[MAXBUFFERBLOCKUNMBER];
char buffer[MAXBUFFERBLOCKUNMBER][MAXLINE];

/*below all about demoF's data structure unblockList/blockList*/
//#define MAXLISTUNMBER 20 //MAXLISTUNMBER = MAXBUFFERBLOCKUNMBER
#define MAXLISTUNMBER  MAXBUFFERBLOCKUNMBER
int inputMutexListUNB[MAXLISTUNMBER + 1];
int inputMutexListB[MAXLISTUNMBER + 1];
int UNBHead;
int UNBTail;
int BHead;
int BTail;
pthread_mutex_t mutexListTable = PTHREAD_MUTEX_INITIALIZER;
#define BLOCKMODE 1
#define UNBLOCKMODE 0
pthread_mutex_t qlock =PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
int requestNum;

/*below is :
 * get one buffer block + put back one buffer block*/
void BufferInit() {
	pthread_mutex_lock(&mutexOccupyTable);
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		if (pthread_mutex_init(mutexBuffer + i, NULL) != 0) {
			printf("can't init the mutex exit!\n");
			exit(0);
		}
	}
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		occupyT[i] = UNOCCUPY;
	}
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		pthread_mutex_lock(mutextBuffer + i);
	}
	pthread_mutex_unlock(&mutexOccupyTable);
}
void BufferDestroy() {
	pthread_mutex_lock(&mutexOccupyTable);
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		pthread_mutex_destroy(mutexBuffer + i);
	}
	pthread_mutex_unlock(&mutexOccupyTable);
}
int getOneBuffer() {
	pthread_mutex_lock(&mutexOccupyTable);
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		if (UNOCCUPY == occupyT[i]) {
			occupyT[i] = OCCUPY;
			return i;
		}
	}
	pthread_mutex_unlock(&mutexOccupyTable);
	return -1;
}
int putbackBuffer(int index) {
	pthread_mutex_lock(&mutexOccupyTable);

	if (UNOCCUPY == occupyT[index]) {
		printf("wrong in putbackBuffer\n");
		exit();
	}
	occupy[index] = OCCUPY;

	pthread_mutex_unlock(&mutexOccupyTable);
	return 0;
}

/*below is demoFunction
 * get one from List no matter block/unblock
 * deal the command stream*/
int submitToDemoF(int index, int blockOrNot) {
	int i = 0;

	pthread_mutex_lock(&qlock);
	if(requestNum >= MAXLISTUNMBER){
		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&qready);
		return -1;
	}
	requestNum++;
	pthread_mutex_unlock(&qlock);
	pthread_cond_signal(&qready);

	if (BLOCKMODE == blockOrNot) {
		while (-1 == inputDemoFList(index, BLOCKMODE)) {
			if (i > MAXWAITTIME)
				return -1;
			sleep(++i);
		}
		pthread_mutext_lock(&mutexBuffer[index]);
	} else if (UNBLOCKMODE == blockOrNot) {
		while (-1 == inputDemoFList(index, UNBLOCKMODE)) {
			if (i > MAXWAITTIME)
				return -1;
			sleep(++i);
		}
	} else {
		return -1;
	}

	return i;
}
void demoFListInit() {
	pthread_mutex_lock(&mutexListTable);
	UNBHead = 0;
	UNBTail = 0;
	BHead = 0;
	BTail = 0;
	pthread_mutex_unlock(&mutexListTable);
	requestNum=0;
	return;
}
void demoFListDestroy() {
	return;
}
int inputDemoFList(int index, int blockOrNot) { //erro return -1;
	int temp = -1;
	pthread_mutex_lock(&mutexListTable);
	if (BLOCKMODE == blockOrNot) {
		if (((BTail + 1) / (MAXBUFFERBLOCKUNMBER + 1)) == BHead) {  //full
#ifdef DEBUGV
			printf("cycle-block-buffer full\n");
#endif
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		} else {
			inputMutexListB[BTail] = index;
			BTail++;
			temp = index;
#ifdef DEBUGV
			printf("cycle-block-buffer not full\n");
#endif
		}
	} else if (UNBLOCKMODE == blockOrNot) {
		if (((UNBTail + 1) / (MAXBUFFERBLOCKUNMBER + 1)) == UNBHead) { //full
#ifdef DEBUGV
			printf("cycle-unblock-buffer full\n");
#endif
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		} else {
			inputMutexListUNB[UNBTail] = index;
			UNBTail++;
			temp = index;
#ifdef DEBUGV
			printf("cycle-block-buffer not full\n");
#endif
		}
	} else {
		pthread_mutex_unlock(&mutexListTable);
		return -1;
	}
	pthread_mutex_unlock(&mutexListTable);
	return temp;
}
int getOneFromDemoFList() {  //erro return -1;
	int temp = -1;
	pthread_mutex_lock(&mutexListTable);
	if (BHead != BTail) {
#ifdef DEBUGV
		printf("get-one-block-buffer\n");
#endif
		temp = inputMutexListB[BHead];
		BHead++;
	} else if (UNBHead != UNBTail) {
#ifdef DEBUGV
		printf("get-one-unblock-buffer\n");
#endif
		temp = inputMutexListUNB[UNBHead];
		UNBHead++;
	} else {
#ifdef DEBUGV
		printf("not get-one-buffer\n");
#endif
		temp = -1;
	}
	pthread_mutex_unlock(&mutexListTable);
	return temp;
}
int getOneFromDemoFList(int ListType) { //erro return -1;
	int temp = -1;
	pthread_mutex_lock(&mutexListTable);
	if (UNBLOCKMODE == ListType) {
		if (UNBHead == UNBTail) {
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		}
		temp = inputMutexListUNB[UNBHead];
		UNBHead++;
		pthread_mutex_unlock(&mutexListTable);
		return temp;
	} else if (BLOCKMODE == ListType) {
		if (BHead == BTail) {
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		}
		temp = inputMutexListB[BHead];
		BHead++;
		pthread_mutex_unlock(&mutexListTable);
		return temp;
	} else {
		pthread_mutex_unlock(&mutexListTable);
		return -1;
	}
	pthread_mutex_unlock(&mutexListTable);
	return -1;
}
int demoF(int index){
	printf("empty! just test !\n");
	return -1;
}
int demoF() {
	int index;
	while(1){
		pthread_mutext_lock(&qlock);
		while(requestNum == 0)
			pthread_cond_wait(&qready,&qlock);
		requestNum --;
		pthread_mutext_unlock(&qlock);

		if((index=getOneFromDemoFList(BLOCKMODE))!=-1){
			demoF(index);
			pthread_mutext_unlock(&mutexBuffer[index]);
		}else if((index=getOneFromDemoFList(UNBLOCKMODE))!=-1){
			demoF(index);
		}
	}
	return 0;
}

/*others*/
void * thr_acceptInput(void *arg) {
	printf("command:\n");
	while (1) {
		fgets(commandline, 4096, stdin);
		int t = strlen(commandline);
		commandline[t - 1] = '\0';
		if (0 == strcmp(commandline, "exit")) {
			pthread_mutex_lock(&globalLock);
			inputNumber++;
			pthread_mutex_unlock(&globalLock);
			exitStatus = 1;
			break;
		} else {
			continue;
		}
	}
#ifdef DEBUGV
	printf("acceptInput thread terminate\n");
#endif
	pthread_exit((void *) 2);
}

void * serverWork(void *arg) {
	int n;

	if ((connfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) == -1) {
		printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
	}
	n = recv(connfd, buff, MAXLINE, 0);
	buff[n] = '\0';
	printf("recv msg from client: %s\n", buff);
	close(connfd);

	return NULL;
}
void * clientReadData(void *arg) {

	printf("read data: %s\n", commandline);

	if (send(sockfdc, "a", 1, 0) < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
}
void * clientWork(void *arg) {

	memset(&servaddrc, 0, sizeof(servaddrc));
	servaddrc.sin_family = AF_INET;
	servaddrc.sin_port = htons(7666);
	if (inet_pton(AF_INET, "192.168.1.131", &servaddrc.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", "192.168.1.131");
		exit(0);
	}

	if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
			< 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	clientReadData (NULL);

	return NULL;
}
void * initialAtBegin(void *arg) {
	int err;
	if (pthread_mutex_init(&globalLock, NULL) != 0) {
		printf("mutex initial fail\n");
		return 0;
	}

//initial for server
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(7666);

	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if (listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	printf("======waiting for client's request======\n");

//initial for client
	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	return NULL;
}

void * beforeReturn(void *arg) {
	close(listenfd);
	close(sockfdc);
	pthread_mutex_destroy(&globalLock);
	return NULL;
}

int main(int argc, char * argv[]) {
	int err;
	void *t;

	initialAtBegin (NULL);

	err = pthread_create(&ntid, NULL, thr_acceptInput, NULL);
	if (err != 0) {
		printf("can't create thread:%s\n", strerror(err));
		exit(0);
	}

#ifdef DEBUGV
	printf("initalAtBegin over\n");
#endif
	while (1) {

#ifdef DEBUGV
		printf("inputNumber %d,exitStatus %d\n", inputNumber, exitStatus);
#endif
		//do server thing
		serverWork(NULL);

		//deal input
		if (inputNumber > 0) {
#ifdef DEBUGV
			printf("inputNumber %d,exitStatus %d\n", inputNumber, exitStatus);
#endif
			if (1 == exitStatus) {
#ifdef DEBUGV
				printf("ready exit while\n");
				printf("inputNumber %d,exitStatus %d\n", inputNumber,
						exitStatus);
#endif
				err = pthread_join(ntid, &t);
				pthread_mutex_lock(&globalLock);
				inputNumber--;
				pthread_mutex_unlock(&globalLock);
				break;
			}
		}

		//do client thing
		clientWork(NULL);
	}

	beforeReturn(NULL);

#ifdef DEBUGV
	printf("main process terminate\n");
#endif

	return 0;
}
