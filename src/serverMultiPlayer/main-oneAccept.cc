#include <iostream>
#include <map>
#include <unistd.h> // fork, close
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libplayerc++/playerc++.h>
#include <sys/wait.h>
#include <pthread.h>

#define MAXLINE 4096
char buff[4096];
//int playerPortMax = 6665;
//int httpPortMax = 8000;
int portMin = 7000;
int portMax = 50000;
int portNow = portMin;
int listenfd, connfd;
//bool increaseOne = true;
int playerPort = portMin;
int httpPort = portMin + 1;
int jcount = 10;
std::map<unsigned int, unsigned int> pphp; //player port + http webserver port
/*std::map<char,int> first;

 first['a']=10;
 first['b']=30;
 first['c']=50;
 first['d']=70;

 std::map<char,int> second (first.begin(),first.end());

 std::map<char,int> third (second);

 std::map<char,int,classcomp> fourth;                 // class as Compare

 bool(*fn_pt)(char,char) = fncomp;
 std::map<char,int,bool(*)(char,char)> fifth (fn_pt); // function pointer as Compare
 */

pthread_t ntid;

void * thr_fnnew(void *arg) {
	printf("in thread\n");

	//execute player
	jcount = 10;
	int l;
	int i = 0;
	int k = strlen("playerRun ");
	while (jcount--) {
		try {
			//execvp("player",)
			printf("shell execute : %s\n", &buff[k]);
			int status = system(&buff[k]);
			if (-1 == status) {
				printf("system exe player error!\n");

				//find '-p'
				l = strlen(buff);
				for (i = 0; i < (l - 1); i++) {
					if (buff[i] == '-' && buff[i + 1] == 'p')
						break;
				}
				if (i < (l - 1)) {
					buff[i] = ' ';
					i++;
					buff[i] = ' ';
					i++;
					while (buff[i] != ' ') {
						i++;
					}
					while (buff[i] > '0' && buff[i] < '9') {
						buff[i] = ' ';
						i++;
					}
					buff[l] = ' ';
					buff[l + 1] = '-';
					buff[l + 2] = 'p';
					buff[l + 3] = ' ';
					portNow++;
					if (portNow < portMin)
						portNow = portMin;
					if (portNow > portMax)
						portNow = portMin;
					playerPort = portNow;
					snprintf(&buff[l + 4], 20, "%d", portNow);
				} else {
					throw 1;
				}
				//find '-h'
				for (i = 0; i < (l - 1); i++) {
					if (buff[i] == '-' && buff[i + 1] == 'h')
						break;
				}
				if (i < (l - 1)) {
					buff[i] = ' ';
					i++;
					buff[i] = ' ';
					i++;
					while (buff[i] != ' ') {
						i++;
					}
					while (buff[i] > '0' && buff[i] < '9') {
						buff[i] = ' ';
						i++;
					}
					buff[l] = ' ';
					buff[l + 1] = '-';
					buff[l + 2] = 'h';
					buff[l + 3] = ' ';
					portNow++;
					if (portNow < portMin)
						portNow = portMin;
					if (portNow > portMax)
						portNow = portMin;
					httpPort = portNow;
					snprintf(&buff[l + 4], 20, "%d", portNow);
				} else {
					throw 2;
				}

			} else {
				pphp[playerPort] = httpPort;
				break;
			}
		} catch (PlayerCc::PlayerError & e) {
			// let's output the error
			std::cerr << e << std::endl;
			printf("throw error when startup player\n");
		} catch (int & ei) {
			printf("command format error %d\n", ei);
		}
	}

	printf("in thread , after execl\n");
	pthread_exit((void *) 2);
}

int dealRequest(int connectfd) {
	int ib=strlen(buff);
	for(int j=0;j<ib;j++){
		if(buff[j]=='\x0d' || buff[j]=='\x0a')
			buff[j]=' ';
	}
	int i = strlen("playerRun");
	char tmp = buff[i];
	buff[i] = '\0';
	if (strcmp(buff, "playerRun") == 0) {
		buff[i] = tmp;

		//find '-p'
		int l = strlen(buff);
		for (i = 0; i < (l - 1); i++) {
			if (buff[i] == '-' && buff[i + 1] == 'p')
				break;
		}
		if (i < (l - 1)) {
			int portVT = atoi(&buff[i + 2]);
			if (portVT > 1024 && portVT < 65535)
				playerPort = portVT;
		} else {
			buff[l] = ' ';
			buff[l + 1] = '-';
			buff[l + 2] = 'p';
			buff[l + 3] = ' ';
			portNow++;
			if (portNow < portMin)
				portNow = portMin;
			if (portNow > portMax)
				portNow = portMin;
			//itoa(playerPortMax,&buff[l+3],10);
			playerPort = portNow;
			snprintf(&buff[l + 4], 20, "%d", portNow);
		}
		//find '-h'
		for (i = 0; i < (l - 1); i++) {
			if (buff[i] == '-' && buff[i + 1] == 'h')
				break;
		}
		if (i < (l - 1)) {
			int portVT = atoi(&buff[i + 2]);
			if (portVT > 1024 && portVT < 65535)
				httpPort = portVT;
		} else {
			l = strlen(buff);
			buff[l] = ' ';
			buff[l + 1] = '-';
			buff[l + 2] = 'h';
			buff[l + 3] = ' ';
			portNow++;
			if (portNow < portMin)
				portNow = portMin;
			if (portNow > portMax)
				portNow = portMin;
			//itoa(httpPortMax,&buff[l+3],10);
			httpPort = portNow;
			snprintf(&buff[l + 4], 20, "%d", portNow);
		}

		int err;
		err = pthread_create(&ntid, NULL, thr_fnnew, NULL);
		if (err != 0) {
			printf("can't create thread:%s\n", strerror(err));
			exit(0);
		}

		//itoa(playerPort, buff, 10);
		snprintf(buff, 20, "%d", playerPort);
		int n = strlen(buff);
		buff[n] = ' ';
		//itoa(httpPort, &buff[n + 1], 10);
		snprintf(&buff[n + 1], 20, "%d", httpPort);
		if (jcount == 0) {
			send(connectfd, "0 0", 3, 0);
		} else {
			send(connectfd, buff, strlen(buff), 0);
		}

		return 0;
	}

	//playerPortSet 6665
	//return if success: 6665
	//return if failure: 6666
	i = strlen("playerPortSet");
	tmp = buff[i];
	buff[i] = '\0';
	if (strcmp(buff, "playerPortSet") == 0) {
		buff[i] = tmp;
		return 0;
	}

	//playerPortRequest
	//return the max available: 6665
	i = strlen("playerPortRequest");
	tmp = buff[i];
	buff[i] = '\0';
	if (strcmp(buff, "playerPortRequest") == 0) {
		buff[i] = tmp;
		return 0;
	}

	//httpserverPortSet
	i = strlen("httpserverPortSet");
	tmp = buff[i];
	buff[i] = '\0';
	if (strcmp(buff, "httpserverPortSet") == 0) {
		buff[i] = tmp;
		return 0;
	}

	i = strlen("httpserverPortRequest");
	tmp = buff[i];
	buff[i] = '\0';
	if (strcmp(buff, "httpserverPortRequest") == 0) {
		buff[i] = tmp;
		return 0;
	}

	i = strlen("quit");
	tmp = buff[i];
	buff[i] = '\0';
	if (strcmp(buff, "quit") == 0) {
		buff[i] = tmp;
		close(connfd);
		close(listenfd);
		return 0;
	}

	/*
	 i=strlen("");
	 tmp=buff[i];
	 buff[i]='\0';
	 if(strcmp(buff,"")==0){
	 buff[i]=tmp;
	 return 0;
	 }*/

	//send(connfd, &buff[0], strlen(buff), 0);
	printf("--3\n");
	send(connectfd, "0 0", 3, 0);
	return 0;
}

int main(int argc, char** argv) {
	struct sockaddr_in servaddr;
	int n;

	if (argc != 2) {
		printf("usage: ./serverOneAccept port\n");
		exit(0);
	}
	int portargv = atoi(argv[1]);

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(portargv);

	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if (listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if ((connfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) == -1) {
		printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
		return 1;
	}

	printf("======waiting for client's request======\n");
	while (1) {
		n = recv(connfd, buff, MAXLINE, 0);
		buff[n] = '\0';
		printf("recv msg from client: %s\n", buff);
		dealRequest(connfd);
		printf("deal one request\n");
		if (n == 0)
			break;
	}
	printf("======server quit======\n");
	close(connfd);
	close(listenfd);
	return 0;
}

