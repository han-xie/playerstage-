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
	sleep(5);

		std::cout<<cyzxic.pos2dGetFrontL()<<std::endl;
		std::cout<<cyzxic.pos2dGetFrontR()<<std::endl;
		std::cout<<cyzxic.Pos2dGetRearL()<<std::endl;
		std::cout<<cyzxic.pos2dGetRearR()<<std::endl;

		unsigned int *pt=new unsigned int[4];
		cyzxic.pos2dGetTires(pt);
		std::cout<<*pt<<" "<<*(pt+1)<<" "<<*(pt+2)<<" "<<*(pt+3)<<std::endl;

	return 0;
}

