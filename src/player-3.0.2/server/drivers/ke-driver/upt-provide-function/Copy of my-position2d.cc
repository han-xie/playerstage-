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
	MFSetPortDirect(0x0000);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	while (1) {
		std::cout << "port:";
		int port;
		std::cin >> port;
		std::cout << MFGetDigiInput(port) << std::endl;
	}

	return 0;
}
