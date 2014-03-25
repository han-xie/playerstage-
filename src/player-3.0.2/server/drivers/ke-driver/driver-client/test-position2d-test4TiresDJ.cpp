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

	cyzxic.bkMFSetServoMode(26, 1);
	sleep(2);
	cyzxic.bkMFSetServoMode(27, 1);
	sleep(2);
	cyzxic.bkMFSetServoMode(28, 1);
	sleep(2);
	cyzxic.bkMFSetServoMode(29, 1);
	sleep(2);

	int t = 0;
	while (1) {
		std::cout << "choose one  3-stop 5-exit  6-speed\n";
		std::cin >> t;
		switch (t) {

		case 3: {
			cyzxic.pos2dSet4Tires(26, 27, 28, 29, 0, 0, 0, 0);
			sleep(1);
			cyzxic.bkMFServoAction();
			break;
		}
		case 5: {
			break;
		}
		case 6: {
			int speed;
			std::cout << "speed:\n";
			std::cin >> speed;

			cyzxic.pos2dSet4Tires(26, 27, 28, 29, speed, speed, speed, speed);
			sleep(1);
			cyzxic.bkMFServoAction();
			break;
		}
		default:
			break;
		}
	}

	return 0;
}
