/*
 * readArgv.cc
 *
 *  Created on: Sep 19, 2013
 *      Author: keevi7c3l
 */

#include <stdio.h>

int main(int argc,char *argv[]){
	printf("argc :%d\n",argc);
	for(int i=0;i<argc;i++){
		printf("argv[%d]:%s\n",i,argv[i]);
	}
	return 0;
}


