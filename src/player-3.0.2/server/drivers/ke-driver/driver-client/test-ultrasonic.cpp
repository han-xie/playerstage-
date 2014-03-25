/*
 * test-dio.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include "CYZXInter.h"

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	PlayerClient robot("192.168.0.111", 6666);
	OpaqueProxy opaquep(&robot, 0);

	CYZXInter cyzxic(&robot, &opaquep);

	//cyzxic.dioSetPortDirect(0x00000000);
	sleep(1);
	for(int i=0;i<100;i++) {
		std::cout<<cyzxic.bkGetUltrasonic()<<std::endl;
		usleep(100);
	}

	return 0;
}

