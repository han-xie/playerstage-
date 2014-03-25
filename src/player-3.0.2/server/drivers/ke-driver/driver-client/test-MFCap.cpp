/*
 * test-MFCap.cpp
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

	sleep(3);
	std::cout << "capseth\n";
	cyzxic.capSetH(50, 0);
	sleep(1);
	std::cout << "capsets\n";
	cyzxic.capSetS(255, 60);
	sleep(2);

	for (int i = 0; i < 10; i++) {
		std::cout<<"i\n";
		int mfcapx = cyzxic.capGetCenterX();
		int mfcapy = cyzxic.capGetCenterY();
		int mfcaps = cyzxic.capGetSum();
		std::cout << "x " << mfcapx << ", y " << mfcapy << ", sum " << mfcaps
				<< "\n";
		usleep(100);
	}

	return 0;
}
