/*
 * socket-serve.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: keevi7c3l
 */

//服务器端
#include <unistd.h> // fork, close
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>


#include <iostream>
#include <stdlib.h> // for atof()
#if !defined (WIN32)
  #include <unistd.h>
#endif

#include <math.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096

#define USAGE \
  "USAGE: goto [-x <x>] [-y <y>] [-h <host>] [-p <port>] [-m]\n" \
  "       -x <x>: set the X coordinate of the target to <x>\n"\
  "       -y <y>: set the Y coordinate of the target to <y>\n"\
  "       -h <host>: connect to Player on this host\n" \
  "       -p <port>: connect to Player on this TCP port\n" \
  "       -m       : turn on motors (be CAREFUL!)"


bool         gMotorEnable(false);
bool         gGotoDone(false);
std::string  gHostname("localhsot");
uint32_t        gPort(8000);
uint32_t        gIndex(0);
uint32_t        gDebug(0);
uint32_t        gFrequency(10); // Hz



void
print_usage(int argc, char** argv)
{
  std::cout << USAGE << std::endl;
}

int
parse_args(int argc, char** argv)
{
  const char* optflags = "h:p:i:d:u:x:y:m";
  int ch;

  while(-1 != (ch = getopt(argc, argv, optflags)))
  {
    switch(ch)
    {
      /* case values must match long_options */
      case 'h':
          gHostname = optarg;
          break;
      case 'p':
          gPort = atoi(optarg);
          break;
      case 'i':
          gIndex = atoi(optarg);
          break;
      case 'd':
          gDebug = atoi(optarg);
          break;
      case 'u':
          gFrequency = atoi(optarg);
          break;
      case 'm':
          gMotorEnable = true;
          break;
      case '?':
      case ':':
      default:
        print_usage(argc, argv);
        return (-1);
    }
  }

  return (0);
}


int main(int argc, char** argv) {
parse_args(argc, argv);
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[4096];
	int n;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
struct sockaddr_in antolope;

	servaddr.sin_addr.s_addr = inet_aton(gHostname.data(),&antolope.sin_addr);
	servaddr.sin_port = htons(gPort);

	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if (listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	printf("======waiting for client's request======\n");
	while (1) {
		if ((connfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) == -1) {
			printf("accept socket error: %s(errno: %d)", strerror(errno),
					errno);
			continue;
		}
		n = recv(connfd, buff, MAXLINE, 0);
		buff[n] = '\0';
		printf("recv msg from client: %s\n", buff);
		close(connfd);
	}

	close(listenfd);
}

