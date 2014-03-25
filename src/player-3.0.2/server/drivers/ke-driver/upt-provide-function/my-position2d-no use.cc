/*
 * my-position2d.cc
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include  "background.h"
int main(int argc, char * argv[]) {
	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	int speed, angle;
	MFSetServoMode(8, 0);
	std::cout << "speed :" << std::endl;
	std::cin >> speed;
	std::cout << "angle :" << std::endl;
	std::cin >> angle;
	MFSetServoPos(8, angle, speed);
	MFServoAction();
	std::cout << "angle : " << MFGetServoPos(8) << std::endl;

	return 0;
}
