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

	int t = 0;
	while (1) {
		std::cout << "choose one 1-turn 2-pos 3-stop 5-exit  6-speed\n";
		std::cin >> t;
		switch (t) {
		case 1: {
			cyzxic.bkMFSetServoMode(26, 1);
			sleep(2);
			cyzxic.bkMFSetServoMode(27, 1);
			sleep(2);
			cyzxic.bkMFSetServoMode(28, 1);
			sleep(2);
			cyzxic.bkMFSetServoMode(29, 1);
			sleep(2);
			cyzxic.pos2dSet4Tires(26, 27, 28, 29, -400, 400, 400, -400);
			sleep(1);
			cyzxic.bkMFServoAction();
			break;
		}
		case 2: {
			int pos;
			cyzxic.bkMFSetServoMode(18, 0);
			sleep(3);
			cyzxic.bkMFSetServoMode(19, 0);
			sleep(3);
			cyzxic.bkMFSetServoMode(20, 0);
			sleep(3);
			cyzxic.bkMFSetServoMode(21, 0);
			sleep(3);
			std::cout << "pos\n";
			std::cin >> pos;
			cyzxic.pos2dSet4TiresPos(18, 19, 20, 21, pos, 400);
			sleep(1);
			cyzxic.bkMFServoAction();
			break;
		}
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
			cyzxic.bkMFSetServoMode(26, 1);
			sleep(2);
			cyzxic.bkMFSetServoMode(27, 1);
			sleep(2);
			cyzxic.bkMFSetServoMode(28, 1);
			sleep(2);
			cyzxic.bkMFSetServoMode(29, 1);
			sleep(2);
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
