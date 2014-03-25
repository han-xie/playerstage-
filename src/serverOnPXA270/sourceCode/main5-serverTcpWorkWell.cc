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
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>

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
//char buff[4096];
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
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
int requestNum;

int inputDemoFList(int index, int blockOrNot);
int getOneFromDemoFList(int ListType);
int getOneFromDemoFList();

/*below is :
 * get one buffer block + put back one buffer block*/
void bufferInit() {
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
		pthread_mutex_lock(mutexBuffer + i);
	}
	pthread_mutex_unlock(&mutexOccupyTable);
}
void bufferDestroy() {
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
			pthread_mutex_unlock(&mutexOccupyTable);
			return i;
		}
	}
	pthread_mutex_unlock(&mutexOccupyTable);
	return -1;
}
int putbackBuffer(int index) {

	pthread_mutex_lock(&mutexOccupyTable);

#ifdef DEBUGV
	printf("putbackBuffer get mutexOccupyTable,index=%d\n", index);
#endif
	if (UNOCCUPY == occupyT[index]) {
		printf("wrong in putbackBuffer ,index=%d\n", index);
		exit(0);
	}
	occupyT[index] = UNOCCUPY;

	pthread_mutex_unlock(&mutexOccupyTable);
	return 0;
}

/*below is demoFunction
 * get one from List no matter block/unblock
 * deal the command stream*/
int submitToDemoF(int index, int blockOrNot) {
	int i = 0;

	if (BLOCKMODE == blockOrNot) {
		while (-1 == inputDemoFList(index, BLOCKMODE)) {
			if (i > MAXWAITTIME)
				return -1;
			sleep(++i);
		}
#ifdef DEBUGV
		printf("block mode ,submit the request,index=%d\n", index);
#endif

		pthread_mutex_lock(&qlock);
		if (requestNum >= MAXLISTUNMBER) {
#ifdef DEBUGV
			printf("can't submitToDemoF because full,number=%d\n", requestNum);
#endif
			pthread_mutex_unlock(&qlock);
			pthread_cond_signal(&qready);
			return -1;
		}
		requestNum++;
#ifdef DEBUGV
		printf("submitToDemoF, now number is=%d\n", requestNum);
#endif
		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&qready);

		pthread_mutex_lock(&mutexBuffer[index]);
#ifdef DEBUGV
		printf("block mode ,the request get response ,index=%d\n", index);
#endif
	} else if (UNBLOCKMODE == blockOrNot) {
		while (-1 == inputDemoFList(index, UNBLOCKMODE)) {
			if (i > MAXWAITTIME)
				return -1;
			sleep(++i);
		}
		pthread_mutex_lock(&qlock);
		if (requestNum >= MAXLISTUNMBER) {
#ifdef DEBUGV
			printf("can't submitToDemoF because full-unblock,number=%d\n",
					requestNum);
#endif
			pthread_mutex_unlock(&qlock);
			pthread_cond_signal(&qready);
			return -1;
		}
		requestNum++;
#ifdef DEBUGV
		printf("submitToDemoF-unblock, now number is=%d\n", requestNum);
#endif
		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&qready);
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
	requestNum = 0;
	pthread_mutex_unlock(&mutexListTable);

	return;
}
void demoFListDestroy() {
	return;
}
int inputDemoFList(int index, int blockOrNot) { //erro return -1;
	int temp = -1;
	pthread_mutex_lock(&mutexListTable);
	if (BLOCKMODE == blockOrNot) {
		if (((BTail + 1) % (MAXBUFFERBLOCKUNMBER + 1)) == BHead) {  //full
#ifdef DEBUGV
			printf("cycle-block-buffer full tail%d head%d\n", BTail, BHead);
#endif
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		} else {
			inputMutexListB[BTail] = index;
			BTail = (BTail + 1) % (MAXBUFFERBLOCKUNMBER + 1);
			temp = index;
#ifdef DEBUGV
			printf("in inputDemoFList BTail=%d,index=%d\n", BTail, index);
			printf("cycle-block-buffer not full\n");
#endif
		}
	} else if (UNBLOCKMODE == blockOrNot) {
		if (((UNBTail + 1) % (MAXBUFFERBLOCKUNMBER + 1)) == UNBHead) { //full
#ifdef DEBUGV
			printf("cycle-unblock-buffer full tail%d head%d\n", UNBTail,
					UNBHead);
#endif
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		} else {
			inputMutexListUNB[UNBTail] = index;
			UNBTail = (UNBTail + 1) % (MAXBUFFERBLOCKUNMBER + 1);
			temp = index;
#ifdef DEBUGV
			printf("in inputDemoFList UNBTail=%d,index=%d\n", UNBTail, index);
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
		BHead = (BHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
	} else if (UNBHead != UNBTail) {
#ifdef DEBUGV
		printf("get-one-unblock-buffer\n");
#endif
		temp = inputMutexListUNB[UNBHead];
		UNBHead = (UNBHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
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
#ifdef DEBUGV
	printf("in getOneFromDemoFList(int)\n");
#endif
	pthread_mutex_lock(&mutexListTable);
	if (UNBLOCKMODE == ListType) {
#ifdef DEBUGV
		printf(
				"in getOneFromDemoFList(int)-unblock,BHead=%d,BTail=%d,ListType=%d\n",
				BHead, BTail, ListType);
#endif
		if (UNBHead == UNBTail) {
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		}
		temp = inputMutexListUNB[UNBHead];
		UNBHead = (UNBHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
		pthread_mutex_unlock(&mutexListTable);
		return temp;
	} else if (BLOCKMODE == ListType) {
#ifdef DEBUGV
		printf(
				"in getOneFromDemoFList(int)-block,BHead=%d,BTail=%d,ListType=%d\n",
				BHead, BTail, ListType);
#endif

		if (BHead == BTail) {
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		}
		temp = inputMutexListB[BHead];
		BHead = (BHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
		pthread_mutex_unlock(&mutexListTable);
		return temp;
	} else {
		pthread_mutex_unlock(&mutexListTable);
		return -1;
	}
	pthread_mutex_unlock(&mutexListTable);
	return -1;
}
int demoF(int index) {
	static int i = 0;
#ifdef DEBUGV
	printf("comming command(now in demoF):%s\n", buffer[index]);
#endif

	buffer[index][0] = 'd';
	buffer[index][1] = 'e';
	buffer[index][2] = 'm';
	buffer[index][3] = 'o';
	buffer[index][4] = 'F';
	buffer[index][5] = ' ';
	buffer[index][6] = 'd';
	buffer[index][7] = 'o';
	buffer[index][8] = 'n';
	buffer[index][9] = 'e';
	buffer[index][10] = ':';
	buffer[index][11] = i++;
	buffer[index][12] = '\0';
#ifdef DEBUGV
	printf("command done(now in demoF):%s\n", buffer[index]);
#endif
	return -1;
}
int demoF() {
	int index;
	while (1) {
		pthread_mutex_lock(&qlock);
		while (requestNum == 0)
			pthread_cond_wait(&qready, &qlock);
#ifdef DEBUGV
		printf("demoF get one command ,so to deal it ,requestNum=%d\n",
				requestNum);
#endif
		requestNum--;
		pthread_mutex_unlock(&qlock);
#ifdef DEBUGV
		printf("after get one command,requestNum=%d\n", requestNum);
#endif
		if ((index = getOneFromDemoFList(UNBLOCKMODE)) != -1) {
			demoF(index);
		} else if ((index = getOneFromDemoFList(BLOCKMODE)) != -1) {
			demoF(index);
#ifdef DEBUGV
			printf("unlock before\n");
#endif
			pthread_mutex_unlock(&mutexBuffer[index]);
#ifdef DEBUGV
			printf("unlock after\n");
#endif
		}
	}
	return 0;
}

void * acceptInput(void *arg) {
	printf("command:\n");
	while (1) {
		fgets(commandline, 4096, stdin);
		int t = strlen(commandline);
		commandline[t - 1] = '\0';
		if (0 == strcmp(commandline, "exit")) {
			break;
		} else {
			continue;
		}
	}
#ifdef DEBUGV
	printf("acceptInput thread terminate\n");
#endif

	return 0;
}

void * thr_serverWork(void *arg) {
	int n;
	int err;
	int index;

	while (1) {
		if ((connfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) == -1) {
			printf("accept socket error: %s(errno: %d)", strerror(errno),
					errno);
		}

		while (1) {
			//get one buffer
			while (-1 == (index = getOneBuffer())) {
				sleep(1);
			}
#ifdef DEBUGV
			printf("get one buffer:%d\n", index);
#endif

			//recv message from client
			if (recv(connfd, buffer[index], MAXLINE, 0) > 0) {
				buffer[index][n] = '\0';
#ifdef DEBUGV
				printf("recv msg from client: %s\n", buffer[index]);
#endif
			} else {
				break;
			}

			//submit to demoF to handle message
#ifdef DEBUGV
			printf("begin submitToDemoF\n");
#endif
			if (-1 == submitToDemoF(index, BLOCKMODE)) {
#ifdef DEBUGV
				printf("server busy can't deal command!\n");
#endif
			}
#ifdef DEBUGV
			printf("submitToDemoF over !\n");
#endif

			//TCP is block , so get the result
#ifdef DEBUGV
			printf("the result message after submit to demoF:%s\n",
					buffer[index]);
#endif

			//put back the buffer
			putbackBuffer(index);
#ifdef DEBUGV
			printf("putbackBuffer index=%d over\n", index);
#endif
		}

		close(connfd);
	}

	return NULL;
}

void * thr_clientWork(void *arg) {

	/*
	 memset(&servaddrc, 0, sizeof(servaddrc));
	 servaddrc.sin_family = AF_INET;
	 servaddrc.sin_port = htons(7667);
	 if (inet_pton(AF_INET, "192.168.1.131", &servaddrc.sin_addr) <= 0) {
	 printf("inet_pton error for %s\n", "192.168.1.131");
	 exit(0);
	 }

	 if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
	 < 0) {
	 printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
	 exit(0);
	 }
	 */

	while (1) {
		printf("read data: %s\n", commandline);
		sleep(2);
		break;
	}

	return NULL;
}

void * thr_demoF(void *arg) {
	demoF();
	return NULL;
}

void * initialAtBegin(void *arg) {

	int err;

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

#ifdef DEBUGV
	printf("======waiting for client's request======\n");
#endif

	//initial for client
	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	bufferInit();
	demoFListInit();

	return NULL;
}

void * beforeReturn(void *arg) {
	close(listenfd);
	close(sockfdc);
	bufferDestroy();
	return NULL;
}

int main(int argc, char * argv[]) {

	int err;
	void *t;

	initialAtBegin (NULL);

	//do server thing
	err = pthread_create(&ntid, NULL, thr_serverWork, NULL);
	if (err != 0) {
		printf("can't create server thread:%s\n", strerror(err));
		exit(0);
	}

	//do client thing
	err = pthread_create(&ntid, NULL, thr_clientWork, NULL);
	if (err != 0) {
		printf("can't create client thread:%s\n", strerror(err));
		exit(0);
	}

	//demoF
	err = pthread_create(&ntid, NULL, thr_demoF, NULL);
	if (err != 0) {
		printf("can't create demo thread:%s\n", strerror(err));
		exit(0);
	}

#ifdef DEBUGV
	printf("three thread creative over\n");
#endif

	//deal input
	acceptInput(NULL);

	beforeReturn(NULL);

	return 0;
}
