/*
 * testInfrProxSen.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include "modIF.h"

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	PlayerClient robot("192.168.0.111", 6666);
	OpaqueProxy opaquep(&robot, 0);
	CYZXInter cyzxic(&robot, &opaquep);
	infrProxSen ifps(&cyzxic);

	for (;;) {
		printf("红外接近传感器值:%d", ifps.getValue());
		sleep(2);
	}

	return 0;
}
