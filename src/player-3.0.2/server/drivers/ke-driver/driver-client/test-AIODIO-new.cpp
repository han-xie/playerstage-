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

	sleep(5);

	cyzxic.infrProxSen();
	cyzxic.infrProxSen(0);
	cyzxic.infrProxSen(1);
	cyzxic.infrProxSen(2);
	cyzxic.infrProxSen(3);

	printf("over!\n");
	return 0;
}

