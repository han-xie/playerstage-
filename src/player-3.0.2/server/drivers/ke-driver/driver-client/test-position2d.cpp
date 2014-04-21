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

	int pos, id , speed;
	while (1) {
		int t;
		std::cout << "choose one  0-exit 1-pos 2-speed:\n";
		std::cin >> t;
		switch (t) {
		case 0:{
			return -1;
			break;
		}
		case 1:{
			std::cout << "servo id :" << std::endl;
			std::cin >> id;
			std::cout << "pos" << std::endl;
			std::cin >> pos;
			std::cout << "speed" <<std::endl;
			std::cin >> speed;
			cyzxic.bkMFSetServoMode(id, 0);
			sleep(1);
			cyzxic.bkMFSetServoPos(id, pos, speed);
			sleep(5);
			cyzxic.bkMFServoAction();
			break;
		}
		case 2:{
			std::cout << "servo id :" << std::endl;
			std::cin >> id;
			std::cout << "speed" <<std::endl;
			std::cin >> speed;
			cyzxic.bkMFSetServoMode(id, 1);
			sleep(1);
			cyzxic.bkMFSetServoRotaSpd(id, speed);
			sleep(5);
			cyzxic.bkMFServoAction();
			break;
		}
		}

	}
	return 0;
}
