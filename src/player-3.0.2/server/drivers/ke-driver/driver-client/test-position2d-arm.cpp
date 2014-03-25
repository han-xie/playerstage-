/*
 * test-position2d.cpp
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

	int pos, id;
	while (1) {
		std::cout << "servo id :" << std::endl;
		std::cin >> id;
		std::cout << "pos" << std::endl;
			std::cin >> pos;

		cyzxic.bkMFSetServoMode(id, 0);
		sleep(1);
		cyzxic.bkMFSetServoPos(id, pos, 400);
		sleep(5);
		cyzxic.bkMFServoAction();
	}
	return 0;
}
