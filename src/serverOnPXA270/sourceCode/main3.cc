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
