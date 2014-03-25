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
	char buff[4096];
	int n;

	if (argc != 3) {
		printf("usage: ./client <ipaddress> port\n");
		exit(0);
	}

	int portargv = atoi(argv[2]);

	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&servaddrc, 0, sizeof(servaddrc));
	servaddrc.sin_family = AF_INET;
	servaddrc.sin_port = htons(portargv);
	if (inet_pton(AF_INET, argv[1], &servaddrc.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", argv[1]);
		exit(0);
	}

	while (1) {
		if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
				< 0) {
			printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
			exit(0);
		}
		printf("send msg to server: \n");
		fgets(sendlinec, 4096, stdin);

		if ((strcmp(sendlinec, "quit") == 0)
				|| (strcmp(sendlinec, "quit\n") == 0)) {
			close(sockfdc);
			exit(0);
		}

		if (send(sockfdc, sendlinec, strlen(sendlinec), 0) < 0) {
			printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
			exit(0);
		}

		n = recv(sockfdc, buff, MAXLINE, 0);
		buff[n] = '\0';
		printf("recv msg from client: %s\n", buff);

		close(sockfdc);
	}
	exit(0);
}
