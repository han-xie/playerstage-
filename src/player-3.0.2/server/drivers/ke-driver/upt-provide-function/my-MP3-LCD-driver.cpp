/*
 * my-position2d.cc
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include  "../background.h"
int main(int argc, char * argv[]) {
	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	int t = 1;
	while (1) {
		std::cout << "choose one 1 or 2\n10 stop\n3 LCD" << std::endl;
		std::cin >> t;
		if (1 == t) {
			MFMp3Play("zombi.mp3", 9);
		} else if (2 == t) {
			MFMp3Play("TheDivingBoard.mp3", 18);
		} else if (10 == t) {
			MFMp3Stop();
		}
		else if(3 ==t){
			MF2X4LCD("hello world", 12);
		}
	}
	return 0;
}
