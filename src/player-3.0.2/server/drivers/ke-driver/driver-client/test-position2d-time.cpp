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
	int id;
	double seconds;

	while (1) {
		std::cout
				<< "choose one 1-go 2-stop 5-exit\n 10-1s 11-2s 12-3s 13-4s 14-5s 15-6s\n";
		std::cin >> t;
		switch (t) {
		case 1: {
			std::cout << "id\n";
			std::cin >> id;
			std::cout << "seconds\n";
			std::cin >> seconds;

			cyzxic.bkMFSetServoRotaSpdTime(id, 400, seconds);

			break;
		}
		case 2: {
			std::cout << "id\n";
			std::cin >> id;
			cyzxic.bkMFSetServoRotaSpd(id, 0);
			break;
		}
		case 5: {
			return 0;
		}
		case 10: {
			std::cout << "id\n";
			std::cin >> id;
			cyzxic.bkMFSetServoRotaSpdTime(id, 400, 1);
			break;
		}
		case 11: {
			std::cout << "id\n";
			std::cin >> id;
			cyzxic.bkMFSetServoRotaSpdTime(id, 400, 2);
			break;
		}
		case 12: {
			std::cout << "id\n";
			std::cin >> id;
			cyzxic.bkMFSetServoRotaSpdTime(id, 400, 3);
			break;
		}
		case 13: {
			std::cout << "id\n";
			std::cin >> id;
			cyzxic.bkMFSetServoRotaSpdTime(id, 400, 4);
			break;
		}
		case 14: {
			std::cout << "id\n";
			std::cin >> id;
			cyzxic.bkMFSetServoRotaSpdTime(id, 400, 5);
			break;
		}
		case 15: {
			std::cout << "id\n";
			std::cin >> id;
			cyzxic.bkMFSetServoRotaSpdTime(id, 400, 6);
			break;
		}
		default:
			break;
		}
	}

	return 0;
}
