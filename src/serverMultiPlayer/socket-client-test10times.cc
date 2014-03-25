/*
 * socket-client.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: keevi7c3l
 */

//客户端
#include <arpa/inet.h>
#include <unistd.h> // fork, close
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXLINE 4096

int main(int argc, char** argv) {
	int sockfdc;
	char recvlinec[4096], sendlinec[4096];
	struct sockaddr_in servaddrc;

	if (argc != 2) {
		printf("usage: ./client <ipaddress>\n");
		exit(0);
	}

	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&servaddrc, 0, sizeof(servaddrc));
	servaddrc.sin_family = AF_INET;
	servaddrc.sin_port = htons(7666);
	if (inet_pton(AF_INET, argv[1], &servaddrc.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", argv[1]);
		exit(0);
	}

	if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
			< 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	printf("send msg to server: \n");
	fgets(sendlinec, 4096, stdin);
	for (int i = 0; i < 30; i++) {
		if (send(sockfdc, sendlinec, strlen(sendlinec), 0) < 0) {
			printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
			exit(0);
		}
		usleep(100);

	}

	close(sockfdc);
	exit(0);
}
