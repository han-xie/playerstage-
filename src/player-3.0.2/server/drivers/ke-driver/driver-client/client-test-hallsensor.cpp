/*
 * client-test-hallsensor.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include "CYZXInter.h"

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	PlayerClient robot("192.168.0.111", 6668);
	OpaqueProxy opaquep(&robot, 0);

	CYZXInter cyzxic(&robot, &opaquep);

	//cyzxic.hallsensorInit(7);
	std::cout<<cyzxic.hallsensor(7)<<std::endl;

	return 0;
}
