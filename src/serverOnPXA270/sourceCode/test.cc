/*
 * main.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: keevi7c3l
 */

#include "apue.h"
#include <unistd.h> // fork, close
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

char commandline[4096];
/*
int myStrcmp(const char*s1, const char*s2) {
	while (*s1 != 0 && *s2 != 0 && *s1 == *s2) {
		printf("!%c! !%c!\n", *s1, *s2);
		s1++;
		s2++;
	}
	if (*s1 != 0)
		printf("s1!%c!\n", *s1);
	if (*s2 != 0)
		printf("s2!%c!\n", *s2);
	return *s1 - *s2;
}

int main(int argc, char * argv[]) {
	char command[5] = { 'e', 'x', 'i', 't', '\0' };
	while (1) {
		fgets(commandline, 4096, stdin);
		int t = strlen(commandline);
		commandline[t-1] = '\0';
		printf("commandline:%s\n", commandline);
		printf("%d\n", myStrcmp(commandline, "exit"));
	}
	return 0;
}*/

int main(int argc, char * argv[]) {
	int listenfd,listenfd2;
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	if ((listenfd2 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	printf("%d %d",listenfd,listenfd2);
	return 0;
}
