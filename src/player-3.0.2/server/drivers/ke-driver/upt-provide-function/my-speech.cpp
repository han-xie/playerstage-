/*
 * my-speech.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include  "background.h"
int main(int argc, char * argv[]) {
	int vIndex = 0;
	MFSRInsertText(0, (char*) &("前进"), 5);
	MFSRInsertText(1, (char*) &("后退"), 5);
	MFSRInsertText(2, (char*) &("forward"), 8);
	MFSRStart();
	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	int t;

	while (1) {
		vIndex=MFSRGetResIndex();
		switch (vIndex) {
		case 0:
			std::cout << "0------------\n" << std::endl;
			break;
		case 1:
			std::cout << "1-------------\n";
			break;
		case 2:
			std::cout << "forward---------------\n";
			break;
		default:
			std::cout << "others-------------\n";
			break;
		}
		DelayMS(100000);
		std::cout<<"any continue-------------\n";
		std::cin>>t;
	}
}

