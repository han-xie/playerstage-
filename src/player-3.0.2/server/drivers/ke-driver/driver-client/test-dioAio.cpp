/*
 * test-dio.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include "CYZXInter.h"
#include "args.h"

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	parse_args(argc, argv);
	PlayerClient robot(gHostname, gPort);
	OpaqueProxy opaquep(&robot, gIndex);

	CYZXInter cyzxic(&robot, &opaquep);

	//cyzxic.dioSetPortDirect(0x00000000);
	sleep(5);
for(;;){
std::cout<<"0 exit 1 dio 2 aio\n";
int j;
std::cin>>j;
switch(j){
case 0:{
return 0;
}
case 1:{
std::cout<<"port\n";
int k;
std::cin>>k;
	for(int i=0;i<5;i++) {
		std::cout<<cyzxic.dioGetDigiInput(k)<<std::endl;
		usleep(100);
	}
break;
}
case 2:{
std::cout<<"port\n";
int k;
std::cin>>k;
	for(int i=0;i<5;i++) {
		std::cout<<cyzxic.aioCom(k)<<std::endl;
		usleep(100);
	}
break;
}
}

}

	return 0;
}

