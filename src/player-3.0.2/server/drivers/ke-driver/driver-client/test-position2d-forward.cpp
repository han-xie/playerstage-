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

			cyzxic.pos2dForward(300);
			break;
		}
		case 2: {

			cyzxic.pos2dBackward(300);
			break;
		}
		case 3: {
			cyzxic.pos2dTurnLeft(300);
			break;
		}
		case 4: {

			cyzxic.pos2dTurnRight(300);
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
