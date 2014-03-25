/*
 * main.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: keevi7c3l
 */

#include "apue.h"
#include <sys/wait.h>

int main(int argc, char * argv[]) {
	pid_t pid;


	//second son
	char t[4]={'-','a','l','\0'};
	const char *p=t;
	if ((pid = fork()) < 0) {
		printf("fork 2 error\n");
	} else if (pid == 0) {
		if (execl("testArgv", "testArgv",t, (char *) 0) < 0)
			printf("exe 2 error\n");
	}

	if (waitpid(pid, NULL, 0) < 0) {
		printf("wait 2 error\n");
	}

	printf("in father ready exit \n");
	return 0;
}
