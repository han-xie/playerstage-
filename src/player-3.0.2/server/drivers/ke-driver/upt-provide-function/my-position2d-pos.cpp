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

	int pos = 0;
	/*
	 void MFSetServoPos(int inID,int inPos,int inSpeed);
	 void MFServoAction();

	 MFSetServoRotaSpd(8,500);
	 MFSetServoRotaSpd(9,-500);


	 std::cout<<"any key stop\n";
	 std::cin>>t;

	 MFSetServoRotaSpd(8,0);
	 MFSetServoRotaSpd(9,0);
	 */

	int id;
	while (1) {

		std::cout << "id :" << std::endl;
		std::cin >> id;
		MFSetServoMode(id, 0);
		std::cout << "pos :" << std::endl;
		std::cin >> pos;
		MFSetServoPos(id, pos, 300);
		MFServoAction();

	}

	return 0;
}
