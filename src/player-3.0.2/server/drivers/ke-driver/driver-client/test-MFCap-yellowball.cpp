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

	std::cout << "capsetyellow\n";
	cyzxic.capSetYellow();

	for (int i = 0; i < 10; i++) {
		int mfcapx = cyzxic.capGetCenterX();
		int mfcapy = cyzxic.capGetCenterY();
		int mfcaps = cyzxic.capGetSum();
		std::cout << "x " << mfcapx << ", y " << mfcapy << ", sum " << mfcaps
				<< "\n";
		usleep(100);
	}

	return 0;
}
