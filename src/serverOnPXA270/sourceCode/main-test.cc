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
	printf("in father\n");
	if ((pid = fork()) < 0) {
		printf("fork error\n");
	} else if (pid == 0) {
		if (execl("/mnt/yaffs/test/MultiFlexNG1", "MultiFlexNG1", (char *) 0) < 0)
			printf("exe error\n");
	}

	if (waitpid(pid, NULL, 0) < 0) {
		printf("wait error\n");
	}

	//second son
	if ((pid = fork()) < 0) {
		printf("fork 2 error\n");
	} else if (pid == 0) {
		if (execl("/mnt/yaffs/test/MultiFlexNG2", "MultiFlexNG2", (char *) 0) < 0)
			printf("exe 2 error\n");
	}

	if (waitpid(pid, NULL, 0) < 0) {
		printf("wait 2 error\n");
	}

	printf("in father ready exit \n");
	return 0;
}
