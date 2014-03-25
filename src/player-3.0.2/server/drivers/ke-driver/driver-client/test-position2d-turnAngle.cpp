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
				<< "choose one 1-forward  2-backward  3-left  4-right  5-stop 6-turnServo\n";
		std::cin >> t;
		switch (t) {
		case 1: {
			cyzxic.pos2dServoModeMoto();
			sleep(2);
			cyzxic.pos2dForward();
			break;
		}
		case 2: {
			int speed;
			std::cout << "speed\n";
			std::cin >> speed;
			cyzxic.pos2dServoModeMoto();
			sleep(2);
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
			cyzxic.pos2dServoModeMoto();
			sleep(2);
			cyzxic.pos2dTurnRight(speed);
			break;
		}
		case 5: {
			cyzxic.pos2dServoModeMoto();
			sleep(2);
			cyzxic.pos2dStop();
			break;
		}
		case 6:{
			int angle,speed;
			std::cout<<"servo angle\n";
			std::cin>>angle;
			std::cout<<"servo speed\n";
			std::cin>>speed;
			cyzxic.pos2dServoModeServo();
			sleep(2);
			cyzxic.pos2dSetServoTurn(angle,speed);
			break;
		}
		default:
			break;
		}
	}

	return 0;
}
