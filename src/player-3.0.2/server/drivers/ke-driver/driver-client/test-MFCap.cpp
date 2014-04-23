/*
 * test-MFCap.cpp
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

	sleep(3);
	std::cout << "capseth\n";
	cyzxic.capSetH(50, 0);
	sleep(1);
	std::cout << "capsets\n";
	cyzxic.capSetS(255, 60);
	sleep(2);
 
char i;
	for (;;) {
		
		int mfcapx = cyzxic.capGetCenterX();
		int mfcapy = cyzxic.capGetCenterY();
		int mfcaps = cyzxic.capGetSum();
		std::cout << "x " << mfcapx << ", y " << mfcapy << ", sum " << mfcaps
				<< "\n";
		usleep(100);
	}

	return 0;
}
