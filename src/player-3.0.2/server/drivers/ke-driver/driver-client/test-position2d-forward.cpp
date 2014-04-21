/*
 * test-position2d.cpp
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

	int t = 0;
	while (1) {
		std::cout
				<< "choose one 1-forward  2-backward  3-left  4-right  5-stop \n";
		std::cin >> t;
		switch (t) {
		case 1: {

			cyzxic.pos2dForward(500);
			break;
		}
		case 2: {

			cyzxic.pos2dBackward(500);
			break;
		}
		case 3: {
			cyzxic.pos2dTurnLeft(500);
			break;
		}
		case 4: {

			cyzxic.pos2dTurnRight(500);
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
