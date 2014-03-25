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
		std::cout
				<< "choose one 1-forward  2-backward  3-left  4-right  5-stop \n";
		std::cin >> t;
		switch (t) {
		case 1: {
			cyzxic.pos2dForward();
			break;
		}
		case 2: {
			int speed;
			std::cout << "speed\n";
			std::cin >> speed;
			cyzxic.pos2dBackward(speed);
			break;
		}
		case 3: {
			cyzxic.pos2dTurnLeft();
			break;
		}
		case 4: {
			int speed;
			std::cout << "speed\n";
			std::cin >> speed;
			cyzxic.pos2dTurnRight(speed);
			break;
		}
		case 5: {
			cyzxic.pos2dStop();
			break;
		}
		default:
			break;
		}
	}

	return 0;
}
