/*
 * test-dio.cpp
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

	sleep(1);
	int i;
	while (1) {
		std::cout << "1 mp3Play\n2 mp3Stop\n3 LCDDisp\n4 exit\n";
		std::cin >> i;
		switch (i) {
		case 1: {
			cyzxic.bkMp3Play("TheDivingBoard.mp3");
			break;
		}
		case 2: {
			cyzxic.bkMp3Stop();
			break;
		}
		case 3: {
			cyzxic.bkLCDdisp("hello");
			break;
		}
		case 4: {
			exit(0);
		}
		default:
			break;
		}
	}

	return 0;
}

